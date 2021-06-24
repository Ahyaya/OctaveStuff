#!/bin/bash

mtComments_arxiv=0;

while true
do
/root/CGI_valvepro/AutoQuery srvip > servers.html
cp servers.html /html/servers.html

mtComments=$(stat --format=%Y comments.txt)
if [ "$mtComments"x != "$mtComments_arxiv"x ];then sed '/^[[:space:]]*$/d' comments.txt | tail -n 128 > no_space.txt;/root/CGI_valvepro/decodeURL no_space.txt comments.html;cp comments.html /html/comments.html;mtComments_arxiv=$(stat --format=%Y comments.txt); fi

sleep 1m
done
