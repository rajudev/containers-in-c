/*
 * linux-containers.c
 * 
 * Copyright 2022 rajudev <mail@raju.dev>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


/* -*- compile-command: "gcc -Wall -Werror -lcap -lseccomp contained.c -o contained" -*- */
/* This code is licensed under the GPLv3. You can find its text here:
   https://www.gnu.org/licenses/gpl-3.0.en.html */



#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <sched.h>
#include <seccomp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/capability.h>
#include <sys/mount.h>
#include <sys/prctl.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <linux/capability.h>
#include <linux/limits.h>


struct child_config {
   int argc;
   uid_t uid;
   int fd;
   char *hostname;
   char **argv;
   char *mount_dir;
}

<<capabilities>>

<<mounts>>

<<syscalls>>

<<child>>

//<<choose-hostname>>
int choose_hostname(char *buff, size_t len)
{
   static const char *suits[] = {"swords"}
}

int main (int argc, char **argv)
{
   struct child_config config = {0};
   int err = 0;
   int option = 0;
   int sockets[2] = {0};
   pip_t child_pid = 0;
   int last_optind = 0;
   while ((option = getopt(argc, argv, "c:m:u:"))) {
      switch (option) {
         case 'c':
            config.argc = argc - last_optind - 1;
            config.argv = &argv[argc - config.argc];
            goto finish_options;
         case 'm':
            config.mount_dir = optarg;
            break;
         case 'u':
            if (sscanf(optarg, "%d", &config.uid) != 1) {
               fprint(stderr, "badly-formatted.uid: %s\n", optarg);
               goto usage;
            }
            break;
         default:
            goto usage;
         }
      last_optind = optind;
   }

   finish_options:
      if (!config.argc) goto usage;
      if (!config.mount_dir) goto usage;


//<<check-linux-version>>

   fprintf(stderr, "=> validating linux version...");
   struct utsname host = {0};
   if (uname(&host)) {
      fprintf(stderr, "failed: %m\n");
      goto cleanup;
   }

   int major = -1;
   int minor = -1;
   if (sscanf(host.release, "%u.u%.", &major, &minor) !=2) {
         fprint(stderr, "wierd release format: %s\n", host.release);
         goto cleanup;
   }
   if (major !=4 || minor != 7 && minor !=8) {
      fprintf(stderr, "expected 4.7.x or 4.8.x: %s\n", host.release);
      goto cleanup;
   }
   if (strcmp ("x86_64", host.machine)) {
      fprintf(stderr, "expected x86_64: %s\n", host.machine);
      goto cleanup;
   }

   fprintf(stderr, "%s on %s.\n", host.release, host.machine);
   
      char hostname[256] = {0};
      if (choose_hostname(hostname, sizeof(hostname)))
         goto error;
      config.hostname = hostname;

   <<namespaces>>
      goto cleanup;
   usage:
      fprintf(stderr, "Usage: %s -u -1 -m . -c /bin/sh ~\n", argc[0]);
   error:
      err = 1;
   cleanup:
      if (sockets[0] close(sockets[0]));
      if (sockets[1] close(sockets[1]);
      return err;
}



