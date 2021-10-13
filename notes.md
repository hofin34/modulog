# Small tools
### HardInfo
Cons:
1. cannot add functionality
1. cannot collect and save specific logs in time 


# Complex tools

### Zabbix
Cons:
1. hard configuration - takes time to all setup - have to install and configure server, database and agents 
1. hard to use - reviews tell, that documentation is bad and you have to use it for long time to understand it
1. uses database to store configuration and data - more difficult to manipulate with it

### Nagios
Cons:
1. Big system requirements: 20 GB of free hard drive space, 2 GB memory
1. hard to configure

### Munin
Cons:
1. set time interval between logs?
1. key-value plugin?
1. mainly for visualisation in graphs - cannot sort messages like DEBUG, INFO, ERR. and cannot set watchdogs (if is value > x - tell me about it in some general log file)

### Datadog
Cons:
1. paid



find info about modules and extending functionality

# TODO
 Zpracovat přehled nástrojů Linuxu pro diagnostiku HW/SW systému (od nízkoúrovňových a specializovaných, např. HardInfo, Checkbox, CPUBurn, MPrime, Memtest86+, lsvpd, až po nástroje typu Zabbix, Nagios, ...) a propojit je s požadavky firmy. 


# Implementation tasks
Create core app, that will create agent, creates TCP connection with it and send data in both directions.

Manage agent - if fails, then print some info and restart and if freezes, then print and restart
	- Core has to send agents ACK requests (if they are not freezed)

Receive log message from agent and log it. Processes will communicate in json format and logs from agent will be in format:
{
	PID:...
	logType: ERR/INFO/DEBUG
	Key:...
	Value:...
	AdditionalInfo:(some message)	
	timestamp
}
if error log received or agent fails/freeze, then give info in general_log.txt and points to folder, where will be more info
Log tree:
- general_log.txt
- Agent1Folder
	- ...
- Agent2Folder
	- ...

Create more agents (depending on config file and send them their config) and manage them (check if their fail is logged etc.)

Create/restart agents in runtime with cmd


 

