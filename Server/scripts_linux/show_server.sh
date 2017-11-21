#!/bin/bash

ps x|grep Server_Debug|grep -v grep
ps x|grep Server_Release|grep -v grep
