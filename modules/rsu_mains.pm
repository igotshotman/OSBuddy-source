package rsu_mains;

# All functions in this module requires these modules
require rsu_java;
require rsu_prm_filehandler;

sub unix_main
{
	# Get the data container
	my $rsu_data = shift;
	
	# Check if user has a preferred Java on Unix
	# If user want to use default-Java then
	if (($rsu_data->preferredjava =~ /default-java/))
	{
		# Use default Java
		$rsu_data->javabin = "java";
		
		# Search for the location of the true default Java binary (not the symlink)
		my $truebinary = rsu_java::unix_find_default_java_binary($rsu_data);
			
		# Replace $javabin with the location of the true binary
		$rsu_data->javabin = "$truebinary";
	}
	# Else if user have set a custom path to a Java binary (most likely Sun/Oracle Java)
	elsif (($rsu_data->preferredjava =~ /^\//))
	{
		# Use the user set Java binary
		$rsu_data->javabin = $rsu_data->preferredjava;
	}
	
	# Get the language setting
	my $params = rsu_prm::parseprmfile($rsu_data);
	
	# Make a variable to contain the Java library path
	my $javalibpath;
	
	# Locate the Java JRE lib folder so we can add it to the library PATH
	$javalibpath = rsu_javalib::unix_findlibrarypath($rsu_data);
	
	# Check if Java can be run in client mode and make sure we use the client mode if available
	# as the client mode gives a HUGE boost in performance compared to the default server mode.
	$rsu_data->javabin = rsu_java::check_for_java_client_mode($rsu_data);
	
	# Pass the Java binary to a variable so we can use it in commands
	my $javabin = $rsu_data->javabin;
	
	# If user enabled Alsa sounds and OS is Linux
	if ($rsu_data->forcealsa =~ /(1|true)/i && $rsu_data->OS =~ /linux/)
	{
		# Run the Java -version command and check if it is OpenJDK or Java (both uses different Alsa fixes)
		$rsu_data->javaversion = `$javabin -version 2>&1`;
		
		# Pass the result to a new variable
		my $javaused_result = $rsu_data->javaversion;
		
		# If Java used is Sun/Oracle Java
		if ($javaused_result =~ /Java\(TM\) SE/i)
		{
			# Make a variable for aoss
			my $aoss = "aoss";
			
			# Check /usr/bin for aoss32
			my $aosstest = `ls /usr/bin | grep aoss32`;
			
			# Check if $javabin contains the -client parameter and aoss32 was found
			if($rsu_data->javabin =~ /-client/ && $aosstest =~ /aoss32/)
			{
				# Use aoss32 instead of aoss
				$aoss = "aoss32";
			}
			
			# Wrap Java inside aoss (Alsa wrapper)
			$rsu_data->javabin = "$aoss ".$rsu_data->javabin;
		}
		# Else we are using OpenJDK
		else
		{
			# Tell OpenJDK to use alsa (aoss does not work as OpenJDK is usually set to use PulseAudio over Alsa)
			$rsu_data->javabin = $rsu_data->javabin." -Djavax.sound.sampled.Clip=com.sun.media.sound.DirectAudioDeviceProvider -Djavax.sound.sampled.Port=com.sun.media.sound.PortMixerProvider -Djavax.sound.sampled.SourceDataLine=com.sun.media.sound.DirectAudioDeviceProvider -Djavax.sound.sampled.TargetDataLine=com.sun.media.sound.DirectAudioDeviceProvider";
		}
	}	
	
	# Display Java version we are using
	print "Launching client using this Java version: \n";
	
	# Display the Java version
	system $rsu_data->javabin." -version 2>&1 && echo";	

	# Tell user what we are doing
	print "Fixing possible OpenGL issues by adding the environment variable\n$javalibpath\n";
				
	# Add the library path to the Java binary command
	$rsu_data->javabin = "$javalibpath ".$rsu_data->javabin;
	
	# Print debug info
	print "\nLaunching the RuneScape Client using this command:\ncd ".$rsu_data->clientdir."/bin && ".$rsu_data->javabin." ".$rsu_data->verboseprms." -cp  $params /share\n\nExecuting the RuneScape Client!\nYou are now in the hands of Jagex.\n\n######## End Of Script ########\n######## Jagex client output will appear below here ########\n\n";
	
	# Execute the RuneScape client (hopefully)
	system "cd ".$rsu_data->clientdir."/bin && ".$rsu_data->javabin." ".$rsu_data->verboseprms." -cp  $params /share 2>&1";
}

1;
