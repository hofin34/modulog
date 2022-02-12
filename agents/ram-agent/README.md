# ram-agent

This agent logs free RAM in MiB.

## Config structure

"id": string, agent identification

"logInterval": int, how often take log (in seconds)

[optional] "freeNotSmallerThan": int, if is RAM smaller than this value, then log an error