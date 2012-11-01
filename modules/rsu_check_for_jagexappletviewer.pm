package rsu_cfjav;

	sub check_for_jagexappletviewer
	{
		# Get the rsu_data container
		my $rsu_data = shift;
		
		# Pass the data to some variables that can be used in commands
		my $cwd = $rsu_data->cwd;
		my $clientdir = $rsu_data->clientdir;
		
		my $jarfileexistcheck = "";

		# Execute ls and pipe the result to grep to check if jagexappletviewer.jar exists
		$jarfileexistcheck = `ls \"$clientdir/bin/\" | grep \"jagexappletviewer.jar\"`;

		# Transfer the result to a new variable
		my $jarcheckresult = $jarfileexistcheck;

		# If jagexappletviewer.jar does not exist then
		if ($jarcheckresult !~ /jagexappletviewer.jar/)
		{
			# Make a variable containing the path to the update script
			my $updatescript = "\"$cwd/runescape-update-client\"";
			
			# Execute the updater
			system "$updatescript";
		}
	}
1;
