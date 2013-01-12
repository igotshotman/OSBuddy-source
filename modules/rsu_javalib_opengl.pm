# This module deals with making sure that OpenGL works with Java on Linux
package rsu_javalib;

sub unix_findlibrarypath
{
	# Gets passed data from the function call
	my $rsu_data = shift;
		
	# Pass the binary to a variable so we can use it in commands
	my $binary = $rsu_data->javabin;
		
	# List up the shared library files used by the Java binary (not the symlink!) and remove unneeded info
	my $lddresult = `ldd $binary | grep libjli.so`;
		
	# Find the library path from the ldd output line, removing whitespaces before and after the path.
	$lddresult =~ s/\s*libjli\.so\s*=>\s+(.*)jli\/libjli\.so\s+\(\S+\)\s*$/$1/;

	# Return the library path for java
	return "LD_LIBRARY_PATH=$lddresult";
}

1;
