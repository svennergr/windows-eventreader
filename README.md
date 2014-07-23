windows-eventreader
=========


> is a NodeJS module and will offer you the possibility to get access to windows EventLogs

Version
----

0.0.7Tech
====

To get things work, you will need:

* [Python 2.7] - to build the module
* [VisualStudio] - to build the module. At least version 2010 Express.


Installation
====

```javascript
npm install windows-eventreader
```


Useage
=====


getLogs()
-----

returns an array of readable EventLogs

```javascript
[ { displayName: 'Anwendung',
    logName: 'Application',
    entries: 9546 },
  { displayName: 'Hardware-Ereignisse',
    logName: 'HardwareEvents',
    entries: 0 },
  { displayName: 'Internet Explorer',
    logName: 'Internet Explorer',
    entries: 0 },
  { displayName: 'Key Management Service',
    logName: 'Key Management Service',
    entries: 0 },
  { displayName: 'Media Center',
    logName: 'Media Center',
    entries: 0 },
  { displayName: 'Microsoft Office Alerts',
    logName: 'OAlerts',
    entries: 62 },
  { displayName: 'PreEmptive',
    logName: 'PreEmptive',
    entries: 0 } ]
```

readLog(logName, count, [timeStampFrom, [timeStampTo]])
-----

returns an array of EventLogEntries

```javascript
[ { message: 'The Software Protection service has stopped.\r\n',
    source: 'Office Software Protection Platform Service',
    time: '07.07.2014 20:12:29',
    displayName: 'Anwendung',
    logName: 'Application',
    entries: 9546,
    category: '(0)',
    entryType: '0',
    userName: '',
    machineName: 'Laptop2' },
  { message: 'Successfully scheduled Software Protection service for re-start at 2014-07-07T20:06:29Z. Reason: GVLK.',
    source: 'Office Software Protection Platform Service',
    time: '07.07.2014 20:12:29',
    displayName: 'Anwendung',
    logName: 'Application',
    entries: 9546,
    category: '(0)',
    entryType: 'Information',
    userName: '',
    machineName: 'Laptop2' } ]
```


License
====

MIT

[Windows Management Framework]:http://www.microsoft.com/de-de/download/details.aspx?id=40855
[Python 2.7]:https://www.python.org/download/releases/2.7.6/
[VisualStudio]:http://www.microsoft.com/visualstudio/eng/downloads#d-2010-express