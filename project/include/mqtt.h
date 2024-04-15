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

typedef struct mqtt_ctx_s
{
	char	clientId[128];
	char	username[128];
	char	password[128];
	char	hosturl[128];
	int		port;
	char	topic[128];
	int		keepalive;
	int		qos;

	char	id[128];
//	char	method[128];
//	char    identifier[128];
//  char    version[128];
} mqtt_ctx_t;

#endif
