#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <rte_log.h>

#include "ethdev_driver.h"
#include "rte_ethdev_netns.h"

#define NETNS_STRING_LEN (RTE_ETH_NETNS_MAX_LEN + 64)

int32_t rte_ethdev_enter_netns(int32_t ns)
{
	int32_t ret = -1;
	int32_t origNs = open("/proc/thread-self/ns/net", O_RDONLY | O_CLOEXEC);
	if (origNs < 0) {
		RTE_ETHDEV_LOG(ERR, "open returned an error when accessing self netns %i," \
							"errno:%i\n",ns, errno);
		return origNs;
	}

	ret = setns(ns,0);
	if (ret < 0)
	{
		RTE_ETHDEV_LOG(ERR, "setns returned an error when entering netns %i," \
							"errno:%i\n",ns, errno);
		return ret;
	}
	return origNs;
}

int32_t rte_ethdev_switch_netns(char * netns, int32_t * deviceNS, int32_t * originalNS)
{
	char ns[NETNS_STRING_LEN];
	int32_t local_deviceNS = -1;
	int32_t local_originalNS = -1;

	if (!netns[0])
		return -1;

	snprintf(ns, NETNS_STRING_LEN, "/var/run/netns/%s", netns);
	local_deviceNS = open(ns, O_RDONLY | O_CLOEXEC);
	if (local_deviceNS < 0)
		return -1;
	*deviceNS = local_deviceNS;
	local_originalNS = rte_ethdev_enter_netns(local_deviceNS);
	if (local_originalNS < 0)
		return -1;
	*originalNS = local_originalNS;
	return 0;
}

int32_t rte_ethdev_exit_netns(int32_t current_netns, int32_t intended_netns)
{
	if (intended_netns < 0)
		return -1;
	if (setns(intended_netns, 0) < 0)
	{
		RTE_ETHDEV_LOG(ERR, "setns returned an error when exiting netns %i &" \
							"entering netns %i, errno:%i\n",
							current_netns, intended_netns, errno);
		return -1;
	}
	close(current_netns);
	close(intended_netns);
	return 0;
}

int32_t
rte_ethdev_netns_get(uint16_t port_id, char *netns)
{
	struct rte_eth_dev *dev;

	RTE_ETH_VALID_PORTID_OR_ERR_RET(port_id, -ENODEV);
	dev = &rte_eth_devices[port_id];

	if (netns == NULL) {
		RTE_ETHDEV_LOG(ERR,
			"Cannot get ethdev port %u configuration to NULL\n",
			port_id);
		return -EINVAL;
	}

	memcpy(netns, dev->data->netns, RTE_ETH_NETNS_MAX_LEN);

	return 0;
}