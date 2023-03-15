#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#define RTE_ETH_NETNS_MAX_LEN (64)

/**
 * Changes the network namespace
 *
 * This function changes the current network namespace to a given one
 * on the last iteration (i.e. when no more matching port is found).
 *
 * It is safe to call this function twice; it will do nothing more.
 *
 * @param ns
 *   Fd to the network namespace intended to be opened
 * @return
 *   Fd to the current network namespace or -1 in case of error
 */
int32_t rte_ethdev_enter_netns(int32_t ns);

/**
 * Switches between network namespaces
 *
 * Given a network namespace as string, opens the current network namespace
 * and also the destination network namespace
 * @param netns
 *   String that identifies the destination network namespace
 * @param deviceNS
 *   Pointer to save the fd of the netns to be opened
 * @param originalNS
 *   Pointer to save the fd of the current netns
 * @return
 *   0 if success
 *   -1 if error
 */
int32_t rte_ethdev_switch_netns(char * netns, int32_t * deviceNS, int32_t * originalNS);

/**
 * Exits a network namespace
 *
 * Given a network namespace as string, opens the current network namespace
 * and also the destination network namespace
 * @param current_netns
 *   Fd for the current netns that will be exited. Fd will be closed in this function
 * @param intended_netns
 *   Fd to the destination network namespace. Fd will be closed in this function
 * @return
 *   0 if success
 *   -1 if error
 */
int32_t rte_ethdev_exit_netns(int32_t current_netns, int32_t intended_netns);

/**
 * Returnes a port's network namespace
 *
 * Returns installed netns of a port if given at startup
 * @param port_id
 *   Port id to retrieve the network namespace
 * @param netns
 *   String to save the network namespace for the given port
 * @return
 *   0 if success
 *   -EINVAL if no netns found
 */
int32_t rte_ethdev_netns_get(uint16_t port_id, char *netns);

#ifdef __cplusplus
}
#endif
