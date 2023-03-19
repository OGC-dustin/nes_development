# lib_os_nrtps
OGC Engineering
Non Real-Time Polled Scheduler ( NRTPS )

* small footprint
    Core logic      ( 438 Bytes )
    Per Event Slot  ( 14 Bytes each ) /* adjustable depending on task count */
* cooperative
    Tasks MUST yield for other tasks to be able to operate
