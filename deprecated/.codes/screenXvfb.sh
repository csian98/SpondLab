#!/bin/bash

DISPLAY=:99 xwd -root -silent | convert xwd:- png:/home/spondlab/jupyter/contents/screenshot.png
