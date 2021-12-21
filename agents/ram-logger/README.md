# ram-logger

This agent logs free RAM in MiB.

## Config structure

"id": agent identification, something like "ram-logger"

"logInterval": int, how often take log (in seconds)

[optional] "freeNotSmallerThan": int, if is RAM smaller than this value, then log an error