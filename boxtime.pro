TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = sub_mixpanel sub_startuplaunch sub_boxtime

sub_mixpanel.subdir = mixpanel-qt
sub_startuplaunch.subdir = startuplaunch-qt
sub_boxtime.subdir = boxtime
