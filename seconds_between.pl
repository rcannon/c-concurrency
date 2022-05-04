#!/usr/bin/perl -w
$file_1 = $ARGV[0];
$file_2 = $ARGV[1];
@days_per_mo = (31,31,28,31,30,31,30,31,31,30,31,30,31);
#@days_per_mo_leap = (31,31,29,31,30,31,30,31,31,30,31,30,31);
@days_since_boy   = (0,0,31,59,90,120,151,181,212,243,273,304,334,365);
@days_since_boyl  = (0,0,31,60,91,121,152,182,213,244,274,305,335,366);
#@days_till_eoy    = (365,334,306,275,245,214,184,153,122,92,61,31,0);
#@days_till_eoyl   = (366,335,306,275,245,214,184,153,122,92,61,31,0);
chomp($times = `ls -latr --full-time $file_1 $file_2`);
@file_times = split('\n',$times);
$start_time = $file_times[0];
$end_time   = $file_times[1];
print $end_time . "\n";
print $start_time . "\n";
if ($start_time =~ m/\S+\s+\d+\s+\S+\s+\S+\s+\d+\s+(\d+)\-(\d+)\-(\d+)\s+(\d+):(\d+):(\d+\.\d+)\s+(\S+)\s+/) {
#    $start_protection = $1;
#    $start_ref_c      = $2;
#    $start_user       = $3;
#    $start_group      = $4;
#    $start_size       = $5;
    $start_year       = $1;
    $start_mo         = $2;
    $start_date       = $3;
    $start_hour       = $4;
    $start_min        = $5;
    $start_sec        = $6;
    $start_dst        = $7;
    if ($end_time =~ m/\S+\s+\d+\s+\S+\s+\S+\s+\d+\s+(\d+)\-(\d+)\-(\d+)\s+(\d+):(\d+):(\d+\.\d+)\s+(\S+)\s+/) {
#	$end_protection = $1;
#	$end_ref_c      = $2;
#	$end_user       = $3;
#	$end_group      = $4;
#	$end_size       = $5;
	$end_year       = $1;
	$end_mo         = $2;
	$end_date       = $3;
	$end_hour       = $4;
	$end_min        = $5;
	$end_sec        = $6;
	$end_dst        = $7;

#    Handle daylight savings difference.
	if ($end_dst != $start_dst) {
	    $dst_hour = ($end_dst - $start_dst)/100;
	    $end_hour = $end_hour + $dst_hour;
	    if ($end_hour == 24) {
		$end_hour = 0;
		$end_date = $end_date + 1;
		if ($end_date > $days_per_mo[$end_mo]) {
		    $end_date = 1;
		    $end_mo = $end_mo + 1;
		    if ($end_mo > 12) {
			$end_mo = 1;
			$end_year = $end_year + 1;
		    }
		}
	    } else  {
		if ($end_hour < 0) {
		    $end_hour = 23;
		    $end_date = $end_date - 1;
		    if ($end_date < 0) {
			$end_mo = $end_mo-1;
			if ($end_mo == 0) {
			    $end_date = 31;
			    $end_mo = 12;
			    $end_year = $end_year - 1;
			} else {
			    $end_date = $days_per_mo[$end_mo];
			    if ($end_mo == 2) {
				if (($end_year % 400) == 0) {
				    $end_date = 29;
				} else {
				    if (($end_year % 4) == 0) {
					if (($end_year % 100) != 0) {
					    $end_date = 29;
					}
				    }
				}
			    }
			}
		    }
		}
	    }
	}
#    print "start_year = $start_year\n";
#    print "start_mo   = $start_mo\n";
#    print "start_date = $start_date\n";
#    print "start_hour = $start_hour\n";
#    print "start_min  = $start_min\n";
#    print "start_sec  = $start_sec\n";
#    print "start_frac = $start_frac\n";
	$delta_sec = $end_sec - $start_sec;
	$bend_min  = $end_min;
	if ($delta_sec < 0) {
	    $delta_sec = $delta_sec + 60;
	    $bend_min = $end_min - 1;
	} 
	$delta_min = $bend_min - $start_min;
	$bend_hour = $end_hour;
	if ($delta_min < 0) {
	    $delta_min = $delta_min + 60;
	    $bend_hour   = $end_hour -1;
	}
	$delta_hour = $bend_hour - $start_hour;
	$bend_date  = $end_date;
	if ($delta_hour < 0) {
	    $delta_hour = $delta_hour + 24;
	    $bend_date = $end_date - 1;
	}
	$bend_mo    = $end_mo;
	$bend_year  = $end_year;
	if ($bend_date == 0) {
	    $bend_mo = $bend_mo - 1;
	    if ($bend_mo == 0) {
		$bend_mo = 12;
		$bend_year = $end_year - 1;
	    }
	    $bend_date = $days_per_mo[$bend_mo];
	}
	# Now if $bend_year == $start year then it suffices to
	# determine which day of the year start and end are on 
	# and subtract them
	$bend_leap = 0;
	if (($bend_year % 400) == 0) {
	    $bend_leap = 1;
	} else {
	    if (($bend_year % 4) == 0) {
		if (($bend_year % 100) != 0 ) {
		    $bend_leap = 1;
		}
	    }
	}
	if ($bend_year == $start_year) {
	    if ($bend_leap == 0) {
	       $end_doy = $days_since_boy[$bend_mo] + $bend_date;
	       $start_doy = $days_since_boy[$start_mo] + $start_date;
	    } else {
	       $end_doy = $days_since_boyl[$bend_mo] + $bend_date;
	       $start_doy = $days_since_boyl[$start_mo] + $start_date;
	    }
	    $delta_days = $end_doy - $start_doy;
	} else {
	    $start_leap = 0;
	    if (($start_year % 400) == 0) {
		$start_leap = 1;
	    } else {
		if (($start_year % 4) == 0) {
		    if (($start_year % 100) != 0 ) {
			$start_leap = 1;
		    }
		}
	    }
	    if ($bend_leap == 0) {
	       $end_doy = $days_since_boy[$bend_mo] + $bend_date;
	    } else {
	       $end_doy = $days_since_boyl[$bend_mo] + $bend_date;
	    }
	    $delta_days = $end_doy;
	    if ($start_leap == 0) {
		$delta_days = $delta_days + 365 - ($days_since_boy[$start_mo] + $start_date);
	    } else {
		$delta_days = $delta_days + 366 - ($days_since_boyl[$start_mo] + $start_date);
	    }
	    for ($i=$start_year+1;$i<$bend_year;$i++) {
		$i_leap = 0;
		if (($i % 400) == 0) {
		    $i_leap = 1;
		} else {
		    if (($i % 4) == 0) {
			if (($i % 100) != 0) {
			    $i_leap = 1;
			}
		    }
		}
		if ($i_leap == 0) {
		    $delta_days += 365;
		} else {
		    $delta_days += 366;
		}
	    } # end for ($i...) 
	} # end else $start_year != $bend_year
	# Now we have $delta_days, $delta_hour, $delta_min and $delta_sec
	$seconds_between = ($delta_days) * 86400 + ($delta_hour * 3600) + ($delta_min * 60) + $delta_sec;
	print "seconds between $file_1 and $file_2 creation is $seconds_between\n";
    }
}

	
