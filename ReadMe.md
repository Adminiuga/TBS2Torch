# Thunder Board Sense 2 Torch Light

`
  slc generate \
      --project-file="/src/TBS2Torch2/TBS2Torch2.slcp" \
      --export-destination=/build/TBS2-Torch \
      --copy-proj-sources --new-project --force \
      --configuration=""
`

Then build it with
`
make -f TBS2Torch2.Makefile release
`