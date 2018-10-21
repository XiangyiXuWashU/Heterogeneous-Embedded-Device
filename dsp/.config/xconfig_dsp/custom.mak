## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,e674 linker.cmd package/cfg/dsp_pe674.oe674

linker.cmd: package/cfg/dsp_pe674.xdl
	$(SED) 's"^\"\(package/cfg/dsp_pe674cfg.cmd\)\"$""\"Z:/MINILOC/OMAPL138/dsp/.config/xconfig_dsp/\1\""' package/cfg/dsp_pe674.xdl > $@
