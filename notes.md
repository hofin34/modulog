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


find info about modules and extending functionality

# TODO
 Zpracovat přehled nástrojů Linuxu pro diagnostiku HW/SW systému (od nízkoúrovňových a specializovaných, např. HardInfo, Checkbox, CPUBurn, MPrime, Memtest86+, lsvpd, až po nástroje typu Zabbix, Nagios, ...) a propojit je s požadavky firmy. 