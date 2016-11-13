TEMPLATE = subdirs
SUBDIRS += lib plugin
OTHER_FILES += rpm/libmce-qt5.spec README
lib.target = lib-target
plugin.depends = lib-target
