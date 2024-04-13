/*********************************************************************************
 *      Copyright:  (C) 2024 Liao Shengli<2928382441@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  main.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(04/04/24)
 *         Author:  Liao Shengli <2928382441@qq.com>
 *      ChangeLog:  1, Release initial version on "04/04/24 12:39:48"
 *                 
 ********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <cJSON.h>
#include <mosquitto.h>

#include "database.h"
#include "ds18b20.h"
#include "logger.h"
#include "packet.h"
#include "proc.h"
#include "mqtt.h"

void my_callback(struct mosquitto *mosq, void *obj, int rc);
int check_sample_time(int interval, time_t *last_time);

void print_usage(char *progname)
{
	printf("Usage: %s [OPTION]...\n",progname);
	printf("-t(--interval): sepcify sample interval.\n");
	printf("-d(--debug): running in debug mode.\n");
	printf("-b(--daemon): set program running on background.\n");
	printf("-h(--help): print this help information.\n");

	printf("\nExample: %s -t 10\n", progname);
	return ;
}


int main (int argc, char *argv[])
{
	int					rv = -1;
	int					daemon_run = 0;
	int					time_flag = 0;
	int					interval = 60;
	time_t				last_time = 0;

	char				*logfile = "my_mqtt.log";
	int					loglevel = LOG_LEVEL_INFO;
	int					logsize = 10;

	conn_para_t			para = {
		"k11n1ktsKW9.DS18B20|securemode=2,signmethod=hmacsha256,timestamp=1712143691890|",
		"DS18B20&k11n1ktsKW9",
		"6d308936f8fed79a5d328090854a4d0e027588a575ffc3b2a58518775e32bdd1",
		"iot-06z00jezfxayfvn.mqtt.iothub.aliyuncs.com",
		1833,
		"sys/k11n1ktsKW9/DS18B20/thing/event/property/set",
		"/sys/k11n1ktsKW9/DS18B20/thing/event/property/post",
		"{\"data\":{\"temperature\":27.32}}",
		60,
		0
	};

	struct mosquitto	*mosq = NULL;
	bool				session = true;
	int					mid;

	char            	pack_buf[1024];
	int					pack_bytes= 0;
	packet_t        	pack;
	pack_proc_t			pack_proc = packet_json;

	int					opt = -1;
	const char      	*optstring = "t:dbh";
	struct option   	opts[] = {
		{"interval", required_argument, NULL, 't'},
		{"debug", no_argument, NULL, 'd'},
		{"daemon", no_argument, NULL, 'b'},
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};

	while( (opt = getopt_long(argc, argv, optstring, opts, NULL)) != -1 )
	{
		switch( opt )
		{
			case 't':
				interval = atoi(optarg);
				break;

			case 'd':
				daemon_run = 0;
				logfile = "console";
				loglevel = LOG_LEVEL_DEBUG;
				break;

			case 'b':
				daemon_run = 1;
				break;

			case 'h':
				print_usage(argv[0]);
				return 0;

			default:
				break;
		}
	}


	if( log_open(logfile, loglevel, logsize, THREAD_LOCK_NONE) < 0 )
	{
		fprintf(stderr, "initial log system failure\n");
		return -1;
	}

	install_signal();
	
	if( daemon_run )
	{
		daemon(1, 0);
	}

	//open_database("my_mqtt.db");

	rv = mosquitto_lib_init();
	if( rv != MOSQ_ERR_SUCCESS )
	{
		log_error("mosquitto init failure: %s\n", strerror(errno));
		rv = -1;
		goto OUT;
	}

	/*新建客户端*/
	mosq = mosquitto_new(para.clientId, true, NULL);
	if( !mosq )
	{
		log_error("create client failure: %s\n", strerror(errno));
		rv = -2;
		goto OUT;
	}
	log_info("create client successfully\n");

	/*配置用户名密码*/
	rv = mosquitto_username_pw_set(mosq, para.username, para.password);
	if( rv != MOSQ_ERR_SUCCESS )
	{
		log_error("set username and password failure: %s\n", strerror(errno));
		rv = -3;
		goto OUT;
	}
	log_info("mqtt set successfully\n");

	log_info("topic: %s\n", para.pub_topic);



#if 1

	while( !g_signal.stop )
	{
#if 0
		time_flag = 0;
		if( check_sample_time(interval, &last_time) )
		{
			log_info("start sampling\n");
			time_flag = 1;

			sample_data(&pack);
			pack_bytes = pack_proc(&pack, pack_buf, sizeof(pack_buf));
			printf("data:%s\n", pack_bytes, pack_buf);
		}
#endif
		mosquitto_connect_callback_set(mosq, my_callback);
	
		rv = mosquitto_connect(mosq, para.hosturl, para.port, para.keepalive);
		log_info("hosturl: %s, port:%d\n", para.hosturl, para.port);
		if( rv != MOSQ_ERR_SUCCESS )
		{
			log_error("connect failure: %s\n", strerror(errno));
			continue;
			sleep(1);
		}
		log_info("connect successfully\n");

		mosquitto_loop_forever(mosq, -1, 1);
		sleep(1);

	}
#endif
	

OUT:
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
	//close_database();
	log_close();

	return 0;
} 


int check_sample_time(int interval, time_t *last_time)
{
	int		flag = 0;
	time_t  now;

	time(&now);
	if( (now - *last_time) >= interval )
	{
		flag = 1;
		*last_time = now;
	}
	return flag;
}


void my_callback(struct mosquitto *mosq, void *obj, int rc)
{
	char            	pack_buf[1024];
	int					pack_bytes= 0;
	packet_t        	pack;
	pack_proc_t			pack_proc = packet_json;

	int					rv = -1;

	conn_para_t			*para;

	sample_data(&pack); 

	pack_bytes = pack_proc(&pack, pack_buf, sizeof(pack_buf));

	para = (conn_para_t *)obj;

	log_info("pack_buf:%s\n", pack_buf);
	if( !rc )
	{
		rv = mosquitto_publish(mosq, NULL, para->pub_topic, pack_bytes, pack_buf, para->qos, NULL);
		if( rv != MOSQ_ERR_SUCCESS)
		{
			log_error("publish failure\n");
			return ;
		}
	}

	mosquitto_disconnect(mosq);
}

