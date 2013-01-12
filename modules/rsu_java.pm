# This modules deals with finding the Java binary to run the client
package rsu_java;

sub findjavabin
{
	# Get the data container
	my $rsu_data = shift;
		
	# Define the variable for javabin
	my $javabin;
		
	# Define a variable to contain a boolean (true/false) to see if OpenJDK exists
	my $openjdk;
		
	# Print debug info
	print "You are running ".$rsu_data->OS.", I will probe for OpenJDK6 or newer\nand use the newest version if possible.\n\n";
			
	# Make a variable to contain the preferredjava so we can use it in the command
	my $preferredjava = $rsu_data->preferredjava;
			
	# run "find /usr/lib/jvm/ -name java" to see if we can find OpenJDK by using grep
	$openjdk = `find -L /usr/lib/jvm/ -name "java" | grep -P "$preferredjava(|-amd64|-i386|-\$\(uname -i\))/bin"`;
			
	# if $openjdk is found (hurray!)
	if ($openjdk =~ /java-\d{1,1}-openjdk(|-\$\(uname -p\)|-i386|-amd64)/)
	{
		# Print debug info
		print "Found OpenJDK files, now checking for the newest installed one.\n";
			
		# Split the string by newline incase OpenJDK-7 was found
		my @openjdkbin = split /\n/, $openjdk;
			
		# Print debug info
		print "Checking which OpenJDK versions are installed...\n\n";
			
		# Run a check to see if we detected OpenJDK7
		my $detectedopenjdk7 = grep { $openjdkbin[$_] =~ /java-\d{1,1}-openjdk-(\$\(uname -p\)|i386|amd64)/ } 0..$#openjdkbin;
			
		# If OpenJDK7 was not found
		if($detectedopenjdk7 =~ /0/)
		{
			# Print debug info
			print "OpenJDK6 detected!, I will use this to run the client!\n";
				
			# We will use OpenJDK6 to launch it (OpenJDK does not have SFX problems like Sun-Java)
			$javabin = "$openjdkbin[0] ";
		}
		else
		{
			# Print debug info
			print "OpenJDK7 detected!, I will use this to run the client!\n";
			
			# Find the index of OpenJDK7
			my @openjdk7index = grep { $openjdkbin[$_] =~ /java-\d{1,1}-openjdk-(\$\(uname -p\)|i386|amd64)/ } 0..$#openjdkbin;

			# We will use OpenJDK7 to launch it (OpenJDK does not have SFX problems like Sun-Java)
			$javabin = "$openjdkbin[$openjdk7index[0]] ";
		}
	}
	else
	{
		# Print debug info
		print "I did not find any version of OpenJDK in /usr/lib/jvm\nI will instead use the default Java in \$PATH\n";
			
		# Ff OpenJDK is not found then we will use default Java (lets pray it is in the $PATH)
		$javabin = "java";
	}

	# Return to call with the Java executable
	return $javabin;
}

sub check_for_java_client_mode
{
	# Gets passed data from the function call
	my $rsu_data = shift;
		
	# Pass the binary to a variable so we can use it in commands
	my $java_binary = $rsu_data->javabin;
		
	# Execute java -help and see if this Java have the -client parameter available
	my $results = `$java_binary -help 2>&1`;
		
	# If the -client parameter is an option
	if ($results =~ /-client/)
	{
		# Tell Java to execute in client mode
		$java_binary = "$java_binary -client";
	}
		
	# Return the results
	return "$java_binary";
}

sub unix_find_default_java_binary
{
	# Get the data container
	my $rsu_data = shift;
		
	# Make a variable for the location of the Java in path
	my $whereisjava;

	# Ask where the Java executable is
	$whereisjava = `whereis java | sed "s/java:\\ //" | sed "s/\\ .*//"`;

	# Make a variable to contain the testing results
	my $test_exec;
		
	# Make a for loop which follows symlinks up to 10 times in order to find the Java binary
	# (you must be really silly if you have 10 symlinks together to point to it!)
	my $counter;
	for ($counter = 0; $counter < 10; $counter++)
	{
		# Check if this is the true binary
		$test_exec = `ls -la $whereisjava`;
			
		# If the result contains "java -> /" then
		if ($test_exec =~ /java\ -.\ \//)
		{
			# Split the result by whitespace
			my @newtest = split(/\ /, $test_exec);
				
			# Replace $whereisjava with the new location to test
			$whereisjava = $newtest[-1];
		}
		else
		{
			# Split the result by whitespace
			my @truebinary = split(/\ /, $test_exec);
				
			# Replace $whereisjava with the new location to test
			$whereisjava = $truebinary[-1];
				
			# Remove the newline from the output
			$whereisjava =~ s/\n//;
				
			# Make sure we end this loop (no point to continue checking)
			$counter = 11;
		}
	}
		
	# Return the true default Java binary
	return "$whereisjava";
}

1;
