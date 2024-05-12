# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/device_controllers
.PATH: ${.CURDIR}/device_controllers/timer
.PATH: ${.CURDIR}/device_controllers/video
.PATH: ${.CURDIR}/device_controllers/kbc

.PATH: ${.CURDIR}/sprites/xpms
.PATH: ${.CURDIR}/viewer

# source code files to be compiled
SRCS = proj.c timer.c kbc.c utils.c keyboard.c mouse.c video.c guiDrawer.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic
CPPFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf 

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
