#include <stdio.h>
#include <unistd.h>

#include <l4/re/env.h>
#include <l4/sys/ipc.h>
#include <l4/sys/vcon.h>

void myPuts(char* str) 
{
	// \n and \0 at the end
    size_t strSize = strlen(str) + 1 + 1;

	l4re_env_t* env = l4re_env();
	l4_msg_regs_t* mr = l4_utcb_mr();
	
	mr->mr[0] = L4_VCON_WRITE_OP;
	mr->mr[1] = strSize;
	strcpy(&mr->mr[2], str);
	((char*)&mr->mr[2])[strSize - 2] = '\n';
	((char*)&mr->mr[2])[strSize - 1] = 0;

	l4_msgtag_t tag, ret;
	tag = l4_msgtag(L4_PROTO_LOG, (strSize + 3) / 4 + 2, 0, 0);

	ret = l4_ipc_send(env->log, l4_utcb(), tag, L4_IPC_NEVER);

	if (l4_msgtag_has_error(ret)) {
		puts("Error happend in myPut function");
	}
}

int main(void)
{
	while(1) {
		myPuts("ABCD!");
		//puts("ABCD!");
		sleep(1);
	}
}
