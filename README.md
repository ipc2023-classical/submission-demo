## IPC Demo Submission

This repository is meant to showcase how to set up an entry for the IPC 2023.
Note that in the root directory of the main branch, there is one Apptainer file
(we could add more to build multiple entries from the same code base).

The file `Apptainer.demo` defines an entry called `demo` and builds the planner
in two stages. Stage 1 is called `build` and is used to compile the planner. The
`%files` section copies all files from the repository into the root directory of
the image. Afterwards, the `%post` section installs build dependencies through
apt and then compiles the planner. We use `strip --strip-all` on the binary to
remove debug symbols and thus make the resulting image smaller.

The second stage in that file is called `run` and defines the image that will
run in the IPC. Its `%files` section copies files from the first stage and here
we only copy the files needed to run the planner. We are using Fast Downward for
this demo, and the files it need to run are the compiled code in
`builds/release/bin/` as well as the driver script (`fast-downward.py` and
`driver`). Try to only copy over necessary files here to keep the image small.

The `%post` step of the second stage installs packages needed to run the
planner, which is only Python in this case. Note that we install
`python3-minimal` instead of `python3` to keep the image small. We also delete
the data generated by `apt-get update` after we are done (`rm -rf /var/lib/apt/lists/*`)
as this is also not needed and increases the image size.

The `%runscript` section then specifies how to execute the planner. In the IPC,
planners are called with three parameters (domain, problem, and a location for
where to write the plan file).

Finally, the `%labels` section defines the meta data of the planner. Please use
the same labels and one line per property.


## License

[Fast Downward](fast-downward.org) is licensed under GPL v3.

```
Fast Downward is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

Fast Downward is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.
```
