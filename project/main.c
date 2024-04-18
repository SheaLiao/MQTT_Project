/*********************************************************************************
 *      Copyright:  (C) 2024 Liao Shengli<2928382441@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  main.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(18/04/24)
 *         Author:  Liao Shengli <2928382441@qq.com>
 *      ChangeLog:  1, Release initial version on "18/04/24 11:02:21"
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

#include "ds18b20.h"
#include "logger.h"
#include "packet.h"
#include "proc.h"
#include "mqtt_conf.h"

#define INI_PATH "./conf/aliyun_conf.ini"

void my_callback(struct mosquitto *mosq, void *obj, int rc);

void print_usage(char *progname)
{
	printf("Usage: %s [OPTION]...\n",progname);
	printf("-d(--debug): running in debug mode.\n");
	printf("-b(--daemon): set program running on background.\n");
	printf("-h(--help): print this help information.\n");

	printf("\nExample: %s -h\n", progname);
	return ;

}


int main (int argc, char **argv)
{
	int					rv = -1;
	int					daemon_run = 0;

	char				*logfile = "my_mqtt.log";
	int					loglevel = LOG_LEVEL_INFO;
	int					logsize = 10;

	mqtt_ctx_t			mqtt;
	struct mosquitto	*mosq;

	int					opt = -1;
	const char			*optstring = "dbh";
	struct option		opts[] = {
		{"debug", no_argument, NULL, 'd'},
		{"daemon", no_argument, NULL, 'b'},
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};

	while( (opt = getopt_long(argc, argv, optstring, opts, NULL)) != -1 )
	{
		switch( opt )
		{
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

	memset(&mqtt, 0, sizeof(mqtt));
	rv = get_mqtt_conf(INI_PATH, &mqtt);

	rv = mosquitto_lib_init();
	if( rv != MOSQ_ERR_SUCCESS )
	{
		log_error("mosquitto init failure:%s\n", strerror(errno));
		rv = -1;
		goto OUT;
	}

	mosq = mosquitto_new(mqtt.clientId, true, (void *)&mqtt);
	if( !mosq )
	{
		log_error("create client failure:%s\n", strerror(errno));
		rv -2;
		goto OUT;
	}

	rv = mosquitto_username_pw_set(mosq, mqtt.username, mqtt.password);
	if( rv != MOSQ_ERR_SUCCESS )
	{
		log_error("set username and password failure: %s\n", strerror(errno));
		rv = -3;
		goto OUT;
	}

	mosquitto_connect_callback_set(mosq, my_callback);

	while( !g_signal.stop)
	{
		rv = mosquitto_connect(mosq, mqtt.hostname, mqtt.port, mqtt.keepalive);
		if( rv != MOSQ_ERR_SUCCESS )
		{
			log_error("connect failure: %s\n", strerror(errno));
			return -4;
			//goto OUT;
		}
		log_info("connect successfully\n");


		mosquitto_loop_forever(mosq, -1, 1);
		sleep(3);
	}

OUT:
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
	log_close();
	return 0;
} 


void my_callback(struct mosquitto *mosq, void *obj, int rc)
{
	int			rv = -1;

	mqtt_ctx_t	*mqtt;
	char		*msg;
	packet_t	pack;

	cJSON		*root;
	cJSON		*item;
	/*only huaweiyun*/
	cJSON		*services;
	cJSON		*properties;

	root = cJSON_CreateObject();
	item = cJSON_CreateObject();

	services = cJSON_CreateArray();
	properties = cJSON_CreateObject();


	if( !mosq || !obj )
	{
		log_error("invalid input argument\n");
		return ;
	}

	mqtt = (mqtt_ctx_t *)obj;

	get_temperature(&pack.temp);

	if( INI_PATH == "./conf/aliyun_conf.ini" )
	{
		cJSON_AddItemToObject(root, "method", cJSON_CreateString(mqtt->method));
		cJSON_AddItemToObject(root, "id", cJSON_CreateString(mqtt->id));
		cJSON_AddItemToObject(root, "params", item);
		cJSON_AddItemToObject(item, "Temperature", cJSON_CreateNumber(pack.temp));
		cJSON_AddItemToObject(root, "version", cJSON_CreateString(mqtt->version));
	}
	else if ( INI_PATH == "./conf/huaweiyun_conf.ini" )
	{
		cJSON_AddItemToObject(root, "method", cJSON_CreateString(mqtt->method));
		cJSON_AddItemToObject(root, "services", services);
		cJSON_AddItemToArray(services, item);
		cJSON_AddItemToObject(item, "id", cJSON_CreateString(mqtt->id));
		cJSON_AddItemToObject(item, "properties", properties);
		cJSON_AddItemToObject(properties, "Temperature", cJSON_CreateNumber(pack.temp));
	}

	msg = cJSON_Print(root);
	log_info("%s\n", msg);

	if( !rc )
	{
		rv = mosquitto_publish(mosq, NULL, mqtt->topic, strlen(msg), msg, mqtt->qos, 0);
		if( rv != MOSQ_ERR_SUCCESS )
		{
			log_error("publish failure:%s\n", strerror(errno));
			return ;
		}
	}

	cJSON_Delete(root);
	free(msg);
	mosquitto_disconnect(mosq);
}
