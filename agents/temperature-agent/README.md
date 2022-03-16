# temperature-agent

This agent reads temperature file specified by "temperatureSource" in config.json and logs it (in that file is just one integer, nothing more)
This file can be on different systems on different places!

## Config structure

"id": string

"temperatureNotSmallerThan": int, logs error if temperature is smaller than this value

"temperatureNotBiggerThan": int, logs error if temperature is bigger than this value

"temperatureSource": string, path to file, where is stored temperature (something like "/sys/class/thermal/thermal_zone1/temp")

"logInterval": int, how often take log (in seconds)