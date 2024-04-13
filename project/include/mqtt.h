/********************************************************************************
 *      Copyright:  (C) 2024 Liao Shengli<2928382441@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  mqtt.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(04/04/24)
 *         Author:  Liao Shengli <2928382441@qq.com>
 *      ChangeLog:  1, Release initial version on "04/04/24 13:00:23"
 *                 
 ********************************************************************************/

#ifndef _MQTT_H_
#define _MQTT_H_

typedef struct conn_para_s
{
	char	clientId[128];
	char	username[128];
	char	password[128];
	char	hosturl[128];
	int		port;
	char	sub_topic[128];
	char	pub_topic[128];
	char	payload[128];
	int		keepalive;
	int		qos;
} conn_para_t;

extern int mqtt_clean(struct mosquitto *mosq);
extern struct mosquitto *mqtt_init(struct mosquitto *mosq, conn_para_t *para);
extern int mqtt_connect(struct mosquitto *mosq, conn_para_t *para);
void my_callback(struct mosquitto *mosq, void *obj, int rc);

#endif
