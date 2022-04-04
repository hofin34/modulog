# linux-system-monitoring-lib-agent 

This agent uses https://github.com/fuxey/Linux-System-Monitoring-Library library.

## Config structure

"id": string, agent identification

"cpuNotBiggerThanPercent": int, log error, if cpu usage is bigger than this value in percent

"ramNotBiggerThanPercent": int, log error, if used ram is bigger than this value in percent

"logInterval": int, how often take log

"statLocation": where is `stat` file located, usually /proc/stat

