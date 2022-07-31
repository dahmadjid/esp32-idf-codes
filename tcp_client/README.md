# TCP Sockets
sockets docs https://pubs.opengroup.org/onlinepubs/007908799/xnsix.html








# Important note on the difference between normal IP Sockets and lwIP sockets
Normal sockets recv() function does not wait for the full amount of data requested to return, it may return earlier. 
You need to provide it with MSG_WAITALL flag to do that.

lwIP sockets used in ESP32 are the inverse, by default they wait for the full amount of data requested.
You need to provide it with MSG_DONTWAIT flag instead to make it behave like normal sockets


details in lwip/sockets.h (at least true for esp idf 4.3.2)

```
/* Flags we can use with send and recv. */
#define MSG_PEEK       0x01    /* Peeks at an incoming message */
#define MSG_WAITALL    0x02    /* Unimplemented: Requests that the function block until the full amount of data requested can be returned */
#define MSG_OOB        0x04    /* Unimplemented: Requests out-of-band data. The significance and semantics of out-of-band data are protocol-specific */
#define MSG_DONTWAIT   0x08    /* Nonblocking i/o for this operation only */
#define MSG_MORE       0x10    /* Sender will send more */
#define MSG_NOSIGNAL   0x20    /* Uninmplemented: Requests not to send the SIGPIPE signal if an attempt to send is made on a stream-oriented socket that is no longer connected. */
```
