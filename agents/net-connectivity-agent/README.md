# net-connectivity-agent
This agent checks, if is connection to the internet and if is possible to send and receive data.
When started, first log is made and then every "checkInterval" is checked, if connection state changed since last log - if so, it is logged.
If not, no log is made.
If connected to the internet, in isInternetConnectivity is value 1. Otherwise 0.

When there is no internet connection, it is logged as error.

## Config structure

"id": string, how will be agent identified (something like "net-connectivity-agent")

"checkInterval": int, how often check actual net state log (in seconds)
