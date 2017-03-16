# Mandatory: Select your BSP and installation prefix
TARGET = arm-rtems4.12
BSP = beagleboneblack
PREFIX = /home/c/development/rtems/rtems-4.12

# Optional: Separate installation base directory
INSTALL_BASE = $(PREFIX)/$(TARGET)/$(BSP)

# Optional: Network test configuration
TEST_RUNNER = $(BSP)
NET_CFG_SELF_IP = 114.212.120.15
NET_CFG_NETMASK = 255.255.240.0
NET_CFG_PEER_IP = 114.212.120.12
NET_CFG_GATEWAY_IP = 114.212.112.1
NET_TAP_INTERFACE = cpsw0
