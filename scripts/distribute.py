#!/usr/bin/python

import argparse
import json
import subprocess
import sys
import os
import time

import signal

from subprocess import PIPE, Popen

fp = open('output', 'w')

def preexec_function(): pass

# setup cmd line args
parser = argparse.ArgumentParser(description='startup script for DecaFS')

parser.add_argument('--config',
                    '-c',
                    type=str,
                    required='True',
                    help='set this flag if a new instance of DecaFS')

args = parser.parse_args()

# load json configuration file
json_data = open(args.config).read()
data = json.loads(json_data)

espresso_nodes=[]
espresso_ips=[]

for node in data["espresso"]:
  espresso_nodes.append(node)
  espresso_ips.append(node["ip"])

print "Copying ../bin/decafs_barsta to ~/decafs_barista"
proc = subprocess.Popen(["scp", "../bin/decafs_barista", "~/decafs_barista"], stdin=open(os.devnull), stdout=sys.stdout, stderr=sys.stderr)
proc.communicate()

for (i, node) in enumerate(espresso_nodes):
   ip = node["ip"]
   node_id = node["id"]
   metadata = node["metadata"]
   filesystem = node["filesystem"]

   print("Copying ..bin/decafs_espresso to " + ip + ":~/decafs_espresso")
   proc = subprocess.Popen(["scp", "../bin/decafs_espresso", ip + ':~/'], stdin=open(os.devnull), stdout=sys.stdout, stderr=sys.stderr)
   proc.communicate()
