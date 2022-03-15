# udp-watchdog-agent

This agent starts listening on `port` and every `checkoutInterval` checks, if all `devices` responded with udp message.
Udp message has format DEVICENAME_TIMESTAMP.
If device didn't respond for the first time, warning is logged.
For the second time is logged error.

## Config structure

"id": string, id of agent,

"port": int, on which port will be server listening

"checkoutInterval": int, how often (in seconds) check, if device responded

"devices": array of strings, each string represents name of the device
