# Arduino-GaragePrimary
Arduino connected to garage door, receives toggle command via MQTT.

## Requires an additional `credentials.h` file to store the following configuration values:
```
#define WIFI_NAME "My Wifi Name"
#define WIFI_PASSWORD "My Wifi Password"

#define MQTT_SERVER "IPv4 address of MQTT server"
#define MQTT_PORT 1883
#define MQTT_USER "MQTT username of this node. Used for auth. i.e. bedroom"
#define MQTT_PASSWORD "MQTT password of this node. Used for auth."
#define MQTT_ID "MQTT ID of this node."
#define MQTT_TOPIC "topic/for-this-node"
```
