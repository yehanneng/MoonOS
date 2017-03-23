//
// Created by Tangruiwen on 2017/3/19.
//

#ifndef MOONOS_MESSAGE_H
#define MOONOS_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

struct mess1 {
    int m1i1;
    int m1i2;
    int m1i3;
    int m1i4;
};

struct mess2 {
    void* m2p1;
    void* m2p2;
    void* m2p3;
    void* m2p4;
};

struct mess3 {
    int	m3i1;
    int	m3i2;
    int	m3i3;
    int	m3i4;
    unsigned long long m3l1;
    unsigned long long m3l2;
    void*	m3p1;
    void*	m3p2;
};

typedef struct _message {
    int source;
    int type;
    union {
        struct mess1 m1;
        struct mess2 m2;
        struct mess3 m3;
    } u;
}MESSAGE;

/* ipc */
#define SEND		1
#define RECEIVE		2
#define BOTH		3	/* BOTH = (SEND | RECEIVE) */

/**
* @enum msgtype
* @brief MESSAGE types
*/
enum msgtype {
    /*
    * when hard interrupt occurs, a msg (with type==HARD_INT) will
    * be sent to some tasks
    */
            HARD_INT = 1,
    /* SYS task */
            GET_TICKS, GET_PID, GET_RTC_TIME,
    /* FS */
            OPEN, CLOSE, READ, WRITE, LSEEK, STAT, UNLINK,
    /* FS & TTY */
            SUSPEND_PROC, RESUME_PROC,
    /* MM */
            EXEC, WAIT,
    /* FS & MM */
            FORK, EXIT,
    /* TTY, SYS, FS, MM, etc */
            SYSCALL_RET,
    /* message type for drivers */
            DEV_OPEN = 1001,
    DEV_CLOSE,
    DEV_READ,
    DEV_WRITE,
    DEV_IOCTL,
    HD_READ,
    HD_WRITE,
    SYS_ALLOC_PAGE,
    SYS_FREE_PAGE

};


/* macros for messages */
#define	FD		u.m3.m3i1
#define	PATHNAME	u.m3.m3p1
#define	FLAGS		u.m3.m3i1
#define	NAME_LEN	u.m3.m3i2
#define	CNT		u.m3.m3i2
#define	REQUEST		u.m3.m3i2
#define	PROC_NR		u.m3.m3i3
#define	DEVICE		u.m3.m3i4
#define	POSITION	u.m3.m3l1
#define	BUF		u.m3.m3p2
#define	OFFSET		u.m3.m3i2
#define	WHENCE		u.m3.m3i3
#define START_LBA 	u.m3.m3i1
#define SECTORS 	u.m3.m3i2

#define	PID		u.m3.m3i2
#define	STATUS		u.m3.m3i1
#define	RETVAL		u.m3.m3i1

#define PAGE_NUM    u.m3.m3i1

#define HD_DEST 1
#define TICK_DEST 2
#define FS_DEST 3

/**
 * user space ss call interface
 * @param function
 * @param src_dest
 * @param msg
 * @return
 */
int send_recv(int function, int src_dest,const MESSAGE* msg);

int get_ticket();

#ifdef __cplusplus
}
#endif

#endif //MOONOS_MESSAGE_H
