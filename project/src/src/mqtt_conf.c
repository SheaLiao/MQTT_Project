/*********************************************************************************
 *      Copyright:  (C) 2024 Liao Shengli<2928382441@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  mqtt_conf.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(16/04/24)
 *         Author:  Liao Shengli <2928382441@qq.com>
 *      ChangeLog:  1, Release initial version on "16/04/24 16:39:00"
 *                 
 ********************************************************************************/

#include "iniparser.h"
#include "dictionary.h"
#include "mqtt_conf.h"
#include "logger.h"
#include <stdio.h>
#include <string.h>

int get_mqtt_conf(char *ini_path, mqtt_ctx_t *mqtt)
{
	dictionary	*ini = NULL;

	const char		*clientId;
	const char		*username;
	const char		*password;
	const char		*hostname;
	int				port;
	const char		*topic;
	int				keepalive;
	int				qos;

	const char		*method;
	const char		*id;
	const char		*temp_id;
	const char		*rh_id;
	const char		*version;

	if(!ini_path || !mqtt)
	{
		printf("invail input parameter in %s\n", __FUNCTION__) ;
		return -1 ;
	}

	ini = iniparser_load(ini_path); 
	if( ini == NULL )
	{
		log_error("iniparser_load() failure\n");
		return -1;
	}

	clientId = iniparser_getstring(ini, "ID:clientId", DEF_CLIENTID);
	username = iniparser_getstring(ini, "user_passwd:username", DEF_USERNAME);
	password = iniparser_getstring(ini, "user_passwd:password", DEF_PASSWORD);
	hostname = iniparser_getstring(ini, "server:hostname", DEF_HOSTNAME);
	port = iniparser_getint(ini, "server:port", DEF_PORT);
	topic = iniparser_getstring(ini, "pub_topic:topic", DEF_TOPIC);
	keepalive = iniparser_getint(ini, "keepalive:keepalive", DEF_KEEPALIVE);
	qos = iniparser_getint(ini, "qos:qos", DEF_QOS);

	method = iniparser_getstring(ini, "json:method", DEF_METHOD);
	id = iniparser_getstring(ini, "json:id", DEF_ID);
	temp_id = iniparser_getstring(ini, "json:temp_id", DEF_TEMP);
	rh_id = iniparser_getstring(ini, "json:rh_id", DEF_RH);
	version = iniparser_getstring(ini, "json:version", DEF_VERSION);


	strncpy(mqtt->clientId, clientId, BUF_SIZE);
	strncpy(mqtt->username, username, BUF_SIZE);
	strncpy(mqtt->password, password, BUF_SIZE);
	strncpy(mqtt->hostname, hostname, BUF_SIZE);
	mqtt->port = port;
	strncpy(mqtt->topic, topic, BUF_SIZE);
	mqtt->keepalive = keepalive;
	mqtt->qos = qos;

	strncpy(mqtt->method, method, BUF_SIZE);
	strncpy(mqtt->id, id, BUF_SIZE);
	strncpy(mqtt->temp_id, temp_id, BUF_SIZE);
	strncpy(mqtt->rh_id, rh_id, BUF_SIZE);
	strncpy(mqtt->version, version, BUF_SIZE);

	iniparser_freedict(ini);

	return 0;
}
