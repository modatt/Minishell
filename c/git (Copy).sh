#!/bin/bash

read -p "Enter msg: " MSG
git add .
git commit -m "$MSG"
git push
