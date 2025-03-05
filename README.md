# finger
Simulazione del famoso comando "finger" di Linux in tutte le sue opzioni e meccanismi con cui estrae i dati dal sistema

FINGER(1)                 BSD General Commands Manual                FINGER(1)

NAME
     finger — user information lookup program

SYNOPSIS
     finger [-lmsp] [user ...] [user@host ...]

DESCRIPTION
     The finger displays information about the system users.

     Options are:

     -s    Finger displays the user's login name, real name, terminal name and
           write status (as a ``*'' after the terminal name if write permis‐
           sion is denied), idle time, login time, office location and office
           phone number.

           Login time is displayed as month, day, hours and minutes, unless
           more than six months ago, in which case the year is displayed
           rather than the hours and minutes.

           Unknown devices as well as nonexistent idle and login times are
           displayed as single asterisks.

     -l    Produces a multi-line format displaying all of the information de‐
           scribed for the -s option as well as the user's home directory,
           home phone number, login shell, mail status, and the contents of
           the files “.plan”, “.project”, “.pgpkey” and “.forward” from the
           user's home directory.

           Phone numbers specified as eleven digits are printed as ``+N-NNN-
           NNN-NNNN''.  Numbers specified as ten or seven digits are printed
           as the appropriate subset of that string.  Numbers specified as
           five digits are printed as ``xN-NNNN''.  Numbers specified as four
           digits are printed as ``xNNNN''.

           If write permission is denied to the device, the phrase ``(messages
           off)'' is appended to the line containing the device name.  One en‐
           try per user is displayed with the -l option; if a user is logged
           on multiple times, terminal information is repeated once per login.

           Mail status is shown as ``No Mail.'' if there is no mail at all,
           ``Mail last read DDD MMM ## HH:MM YYYY (TZ)'' if the person has
           looked at their mailbox since new mail arriving, or ``New mail re‐
           ceived ...'', ``  Unread since ...'' if they have new mail.

     -p    Prevents the -l option of finger from displaying the contents of
           the “.plan”, “.project” and “.pgpkey” files.

     -m    Prevent matching of user names.  User is usually a login name; how‐
           ever, matching will also be done on the users' real names, unless
           the -m option is supplied.  All name matching performed by finger
           is case insensitive.

     If no options are specified, finger defaults to the -l style output if
     operands are provided, otherwise to the -s style.  Note that some fields
     may be missing, in either format, if information is not available for
     them.

     If no arguments are specified, finger will print an entry for each user
     currently logged into the system.
