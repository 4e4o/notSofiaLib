#!/bin/bash 

scp dump_umap_target.sh dvr:~/
ssh -t dvr './dump_umap_target.sh'
scp -r dvr:~/umap_dump .
ssh -t dvr 'rm dump_umap_target.sh'
ssh -t dvr 'rm -rf umap_dump'
