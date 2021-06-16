%: %.in
	sed -e "s/@LIB_EXT@/$(LIB_EXT)/g" "$^" > "$@"

%.so: %.c
	$(CC) -shared -o "$@" $$(pkg-config --cflags lv2) -DNXS_URI='"$(NXS_URI)"' $(CFLAGS) "$^" -lm $$(pkg-config --libs lv2) $(LDFLAGS)
