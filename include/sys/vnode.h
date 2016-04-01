#ifndef _SYS_VNODE_H_
#define _SYS_VNODE_H_

enum vtype{VNON, VREG, VDIR, VBLK, VCHR, VLNK, VSOCK, VFIFO, VBAD};

struct vnode
{
    enum        vtype v_type;           /* vnode type */
    unsigned    v_flag;                 /* vnode flags */
    unsigned    v_usecount;             /* reference count of users */
    unsigned    v_writecount;           /* reference count of writers */

};

#endif // _SYS_VNODE_H_
