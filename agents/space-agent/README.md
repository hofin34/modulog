# space-agent

This agent logs free and available space in specified folders or mounted partitions

## Config structure

"id": string, agent identification

"logInterval": int, how often take log (in seconds)

"foldersToMonitor": array of strings, which folders will be monitored

"partitionsToMonitor": array of strings, which partitions will be monitored - they must be mounted! (mount folder will be detected automatically)

"availableNotSmallerThanPercent": float, if is available space smaller than X percents, then log error
