TEMPLATE = subdirs

SUBDIRS += \
    application \
    engine \
    rules \
    settings

engine.depends = rules
application.depends = settings rules engine
