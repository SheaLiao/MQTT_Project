/*********************************************************************************
 *      Copyright:  (C) 2024 Liao Shengli<2928382441@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  mqtt.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(04/04/24)
 *         Author:  Liao Shengli <2928382441@qq.com>
 *      ChangeLog:  1, Release initial version on "04/04/24 12:57:57"
 *                 
 ********************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <mosquitto.h>

#include "mqtt.h"
#include "logger.h"


int mqtt_clean(struct mosquitto *mosq)
{
	mosquitto_destroy(mosq);	//释放客户端
	mosquitto_lib_cleanup();		//清除
	return 0;
}
#if 0

struct mosquitto *mqtt_init(struct mosquitto *mosq, conn_para_t *para)
{
	int		rv = -1;
	int		mid;

	/*初始化*/
	rv = mosquitto_lib_init();
	if( rv != MOSQ_ERR_SUCCESS )
	{
		log_error("mosquitto init failure: %s\n", strerror(errno));
		mqtt_clean(mosq);
		rv = -1;
	}

	/*新建客户端*/
	mosq = mosquitto_new(para->clientId, true, NULL);
	if( !mosq )
	{
		log_error("create client failure: %s\n", strerror(errno));
		mqtt_clean(mosq);
		rv = -2;
	}

	/*配置用户名密码*/
	rv = mosquitto_username_pw_set(mosq, para->username, para->password);
	if( rv != MOSQ_ERR_SUCCESS )
	{
		log_error("set username and password failure: %s\n", strerror(errno));
		mqtt_clean(mosq);
		rv = -3;
	}
	log_info("mqtt set successfully\n");

#if 0
	/*订阅主题*/
	rv = mosquitto_subscribe(mosq, &mid, para->sub_topic, para->qos);
	if( rv != MOSQ_ERR_SUCCESS )
	{
		log_error("subscribe failure: %s\n", strerror(errno));
		mqtt_clean;
		rv = -6;
	}
#endif
	mosquitto_message_callback_set(mosq, my_callback);
	
	return mosq;
}


int mqtt_connect(struct mosquitto *mosq, conn_para_t *para)
{
	int		rv = -1;

	/*连接服务器*/
	rv = mosquitto_connect(mosq, para->hosturl, para->port, para->keepalive);
	if( rv != MOSQ_ERR_SUCCESS )
	{
		log_error("connect failure: %s\n", strerror(errno));
		mqtt_clean(mosq);
		rv = -4;
	}

	/*线程支持*/
	rv = mosquitto_loop_start(mosq);
	if( rv != MOSQ_ERR_SUCCESS )
	{
		log_error("set loop failure: %s\n", strerror(errno));
		mqtt_clean(mosq);
		rv = -5;
	}


	return 0;
}

void my_callback(struct mosquitto *mosq, void *obj, int rc)
{
        char                    pack_buf[1024];
        int                                     pack_bytes= 0;
        packet_t                pack;
        pack_proc_t                     pack_proc = packet_json;

        int                                     rv = -1;

    sample_data(&pack);

        pack_bytes = pack_proc(&pack, pack_buf, sizeof(pack_buf));

        rv = mosquitto_publish(mosq, NULL, "temp", pack_bytes, pack_buf, 0, NULL);
        if( rv != MOSQ_ERR_SUCCESS)
        {
                log_error("publish failure\n");
                mqtt_clean(mosq);
                return ;
        }

        mosquitto_disconnect(mosq);
}
#endif
