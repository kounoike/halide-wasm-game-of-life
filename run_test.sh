#!/bin/bash

HTML=$1
shift

# Chrome
# なぜかremote-debugging-portをつけないと動かない。rootのときは--no-sandboxも付ける
# exec emrun --browser chrome --browser_args="--headless --no-sandbox --disable-gpu --remote-debugging-port=9222" --kill_exit ${HTML} -- "$@" 2>/dev/null
exec emrun --browser chrome --browser_args="--headless --disable-gpu --remote-debugging-port=9222" --kill_exit ${HTML} -- "$@" 2>/dev/null

# Firefox
# Firefoxは多重起動できないので念のためstartでもkillしておく
# exec emrun --browser firefox --browser_args="--headless" --kill_start --kill_exit ${HTML} -- "$@"
