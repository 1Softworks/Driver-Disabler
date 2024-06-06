# Driver-Disabler

## Overview

DriverDisabler is a tool designed to prevent anti-cheat systems from loading their drivers by injecting return statements into the `StartServiceA` and `OpenServiceW` functions. This effectively blocks the initialization of these drivers during gameplay.

## Instructions

Anti-cheat programs often load their drivers directly from within the game. By injecting return statements into the relevant functions, DriverDisabler ensures these functions execute without starting the anti-cheat services, thereby disabling them.

This tool can be modified to target and disable various anti-cheat services that use different methods for initialization.


## Integration with Other Methods

- Execute DriverDisabler before the anti-cheat system is fully loaded to ensure it effectively disables the anti-cheat drivers.
