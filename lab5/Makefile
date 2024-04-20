# name of the program (Minix service)
PROG=lab5

# source code files to be compiled
SRCS = lab5.c video.c kbc.c keyboard.c timer_utils.c utils.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic
CPPFLAGS += -pedantic -DLAB4

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf -D LCOM_OPTIMIZED

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
