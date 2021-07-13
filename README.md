# lightweight-pid-IPC
When the target process is specified with the PID, the communication should not involved general purpose locking or communication channels. It is better to have a direct synchronous communication model and mechanism. It can be basis of a high performance RPC library for Linux OS.


## Build and Install Module

```
make
insmod lpidipc.ko
```

## Build Reader/Writer

```
gcc reader.c -o reader
gcc writer.c -o writer
```
