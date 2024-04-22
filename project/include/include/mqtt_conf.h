/********************************************************************************
 *      Copyright:  (C) 2024 Liao Shengli<2928382441@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  mqtt_conf.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(04/04/24)
 *         Author:  Liao Shengli <2928382441@qq.com>
 *      ChangeLog:  1, Release initial version on "04/04/24 13:00:23"
 *                 
 ********************************************************************************/

#ifndef _MQTT_CONF_H_
#define _MQTT_CONF_H_

#define BUF_SIZE 128

#define DEF_CLIENTID 	"k11n1ktsKW9.DS18B20|securemode=2,signmethod=hmacsha256,timestamp=1712143691890|"
#define DEF_USERNAME 	"DS18B20&k11n1ktsKW9"
#define DEF_PASSWORD "6d308936f8fed79a5d328090854a4d0e027588a575ffc3b2a58518775e32bdd1"
#define DEF_HOSTNAME 	"iot-06z00jezfxayfvn.mqtt.iothub.aliyuncs.com"
#define DEF_PORT 		1883
#define DEF_TOPIC		"/sys/k11n1ktsKW9/DS18B20/thing/event/property/post"
#define DEF_KEEPALIVE	60
#define DEF_QOS			0
#define DEF_METHOD		"thing.event.property.post"
#define DEF_ID			"12345"
#define DEF_TEMP		"Temperature"
#define DEF_RH			"Humidity"
#define DEF_VERSION		"1.0.0"


typedef struct mqtt_ctx_s
{
	char	clientId[BUF_SIZE];
	char	username[BUF_SIZE];
	char	password[BUF_SIZE];
	char	hostname[BUF_SIZE];
	int		port;
	char	topic[BUF_SIZE];
	int		keepalive;
	int		qos;

	char	method[BUF_SIZE];
	char	id[BUF_SIZE];
	char	temp_id[BUF_SIZE];
	char	rh_id[BUF_SIZE];
	char    version[BUF_SIZE];
} mqtt_ctx_t;

extern int get_mqtt_conf(char *ini_path, mqtt_ctx_t *mqtt);

#endif
