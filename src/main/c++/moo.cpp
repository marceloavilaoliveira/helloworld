#include <string.h>
#include <stdlib.h>
#include <Logger.hpp>

#include <gmpls_protocols.h>

#include "XcpLogger.hpp"
#include "OspfHandlerMarben.hpp"

namespace Cpqd {
namespace Xcp {

void ospf_read_interface_from_context(OspfInterfaceCtx interfaceCtx,
    OspfInterface & interface) {
    gmpls_ospf_get_interface_area(interfaceCtx, &interface.area);
    gmpls_ospf_get_interface_cost(interfaceCtx, &interface.cost);
    gmpls_ospf_get_interface_dead_interval(interfaceCtx,
            &interface.deadInterval);
    gmpls_ospf_get_interface_hello_interval(interfaceCtx,
            &interface.helloInterval);
    gmpls_ospf_get_interface_local_ip(interfaceCtx, &interface.localIp);
    gmpls_ospf_get_interface_mtu(interfaceCtx, &interface.mtu);
    gmpls_ospf_get_interface_priority(interfaceCtx, &interface.priority);
    gmpls_ospf_get_interface_netmask(interfaceCtx, &interface.netmask);
    gmpls_ospf_get_interface_state(interfaceCtx, &interface.interfaceState);
    gmpls_ospf_get_interface_subnet_type(interfaceCtx, &interface.subnetType);
}
OspfHandlerMarben::OspfHandlerMarben() {
    m_IsOspfStarted = 0;
    mp_GmplsControllers = NULL;

}

OspfHandlerMarben::~OspfHandlerMarben() {
//    stopOspf();
}

t_result OspfHandlerMarben::setRouterId(const in_addr routerId) {
    CCLOG_ENTER_FUNCTION(XcpLogger::MOD_OSPF);
    t_result ret = OSPF_SUCCESS;

    strtol(NULL, NULL, 233);
    // >>>>>
    // Sanity checks
    // >>>>>
        if (mp_GmplsControllers == NULL) {
        ret = ERROR_OSPF_MARBEN_CONTROLLERS_NOT_SET;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }

    if (!mp_GmplsControllers->isStackStarted()) {
        ret = ERROR_OSPF_INVALID_STATE;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }

    // >>>>>
    // End of sanity checks
    // >>>>>

    ret = gmpls_ospf_set_router_id(routerId);

    CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
    return ret;
}

t_result OspfHandlerMarben::getRouterId(in_addr& routerId) {
    CCLOG_ENTER_FUNCTION(XcpLogger::MOD_OSPF);
    t_result ret = OSPF_SUCCESS;

    // >>>>>
    // Sanity checks
    // >>>>>
    if (mp_GmplsControllers == NULL) {
        ret = ERROR_OSPF_MARBEN_CONTROLLERS_NOT_SET;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }

    if (!mp_GmplsControllers->isStackStarted()) {
        ret = ERROR_OSPF_INVALID_STATE;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }
    // >>>>>
    // End of sanity checks
    // >>>>>

    ret = gmpls_ospf_get_router_id(&routerId);

    CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
    return ret;
}

t_result OspfHandlerMarben::addArea(const in_addr areaId) {
    CCLOG_ENTER_FUNCTION(XcpLogger::MOD_OSPF);
    t_result ret = OSPF_SUCCESS;

    // >>>>>
    // Sanity checks
    // >>>>>
    if (mp_GmplsControllers == NULL) {
        ret = ERROR_OSPF_MARBEN_CONTROLLERS_NOT_SET;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }

    if (!mp_GmplsControllers->isStackStarted()) {
        ret = ERROR_OSPF_INVALID_STATE;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }
    // >>>>>
    // End of sanity checks
    // >>>>>

    std::vector<in_addr> currAreas;

    ret = getAreas(currAreas);
    if (ret == OSPF_SUCCESS) {
        for (uInt32 i = 0; i < currAreas.size(); ++i) {
            if (currAreas[i].s_addr == areaId.s_addr) {
                ret = ERROR_OSPF_ALREADY_EXISTS;
                return ret;
            }
        }
    }

    ret = gmpls_ospf_add_area(areaId);

    CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
    return ret;
}

t_result OspfHandlerMarben::removeArea(const in_addr areaId) {
    CCLOG_ENTER_FUNCTION(XcpLogger::MOD_OSPF);
    t_result ret = OSPF_SUCCESS;

    // >>>>>
    // Sanity checks
    // >>>>>
    if (mp_GmplsControllers == NULL) {
        ret = ERROR_OSPF_MARBEN_CONTROLLERS_NOT_SET;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }

    if (!mp_GmplsControllers->isStackStarted()) {
        ret = ERROR_OSPF_INVALID_STATE;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }
    // >>>>>
    // End of sanity checks
    // >>>>>

    ret = gmpls_ospf_delete_area(areaId);

    CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
    return ret;
}

t_result OspfHandlerMarben::getAreas(std::vector<in_addr>& areaIds) {
    CCLOG_ENTER_FUNCTION(XcpLogger::MOD_OSPF);
    t_result ret = OSPF_SUCCESS;


    // >>>>>
    // Sanity checks
    // >>>>>
    if (mp_GmplsControllers == NULL) {
        ret = ERROR_OSPF_MARBEN_CONTROLLERS_NOT_SET;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }

    if (!mp_GmplsControllers->isStackStarted()) {
        ret = ERROR_OSPF_INVALID_STATE;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }
    // >>>>>
    // End of sanity checks
    // >>>>>

    t_list areaList;
    t_list_elem * listElem = NULL;
    in_addr * area = NULL;

    areaIds.clear();

    list_init_list(&areaList);

    ret = gmpls_ospf_get_areas(&areaList);

    if (ret == OSPF_SUCCESS) {
		while (list_remove_front(&areaList, (void**)&area) == LIST_SUCCESS) {
			// Add a copy to the list
			areaIds.push_back((*area));

			// Deallocate the area
			free(area);
		}
    }

    // Finish the list
    list_end_list(&areaList);


    CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
    return ret;
}

t_result OspfHandlerMarben::addInterface(const OspfInterface interface) {
    CCLOG_ENTER_FUNCTION(XcpLogger::MOD_OSPF);

    t_result ret = GMPLS_OSPF_SUCCESS;

    // >>>>>
    // Sanity checks
    // >>>>>
    if (mp_GmplsControllers == NULL) {
        ret = ERROR_OSPF_MARBEN_CONTROLLERS_NOT_SET;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }

    if (!mp_GmplsControllers->isStackStarted()) {
        ret = ERROR_OSPF_INVALID_STATE;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }
    // >>>>>
    // End of sanity checks
    // >>>>>

    OspfInterfaceCtx interfaceCtx = NULL;

    ret = gmpls_ospf_create_interface_context(&interfaceCtx);
    if (ret == GMPLS_OSPF_RESOURCE_SHORTAGE) {
        // Maybe it's good here to throw and exception.
        ret = ERROR_OSPF_RESOURCE_SHORTAGE;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }

    ret = gmpls_ospf_set_interface_area(interfaceCtx, interface.area);
    ret = gmpls_ospf_set_interface_cost(interfaceCtx, interface.cost);
    ret = gmpls_ospf_set_interface_dead_interval(interfaceCtx,
            interface.deadInterval);
    ret = gmpls_ospf_set_interface_hello_interval(interfaceCtx,
            interface.helloInterval);
    ret = gmpls_ospf_set_interface_local_ip(interfaceCtx, interface.localIp);
    ret = gmpls_ospf_set_interface_mtu(interfaceCtx, interface.mtu);
    ret = gmpls_ospf_set_interface_priority(interfaceCtx, interface.priority);
    ret = gmpls_ospf_set_interface_subnet_type(interfaceCtx, interface.subnetType);

    ret = gmpls_ospf_add_interface(interfaceCtx);
    if (ret != GMPLS_OSPF_SUCCESS) {
        /** An error occured while creating the interface */
        gmpls_ospf_destroy_interface_context(&interfaceCtx);
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }

    gmpls_ospf_destroy_interface_context(&interfaceCtx);
    CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
    return ret;

}

t_result OspfHandlerMarben::removeInterface(const in_addr ip) {
    CCLOG_ENTER_FUNCTION(XcpLogger::MOD_OSPF);
    t_result ret = OSPF_SUCCESS;


    // >>>>>
    // Sanity checks
    // >>>>>
    if (mp_GmplsControllers == NULL) {
        ret = ERROR_OSPF_MARBEN_CONTROLLERS_NOT_SET;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }

    if (!mp_GmplsControllers->isStackStarted()) {
        ret = ERROR_OSPF_INVALID_STATE;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }
    // >>>>>
    // End of sanity checks
    // >>>>>

    ret = gmpls_ospf_remove_interface(ip);

    CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
    return ret;
}

t_result OspfHandlerMarben::enableInterface(const in_addr ip) {
    CCLOG_ENTER_FUNCTION(XcpLogger::MOD_OSPF);
    t_result ret = OSPF_SUCCESS;


    // >>>>>
    // Sanity checks
    // >>>>>
    if (mp_GmplsControllers == NULL) {
        ret = ERROR_OSPF_MARBEN_CONTROLLERS_NOT_SET;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }

    if (!mp_GmplsControllers->isStackStarted()) {
        ret = ERROR_OSPF_INVALID_STATE;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }
    // >>>>>
    // End of sanity checks
    // >>>>>

    ret = gmpls_ospf_enable_interface(ip);

    CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
    return ret;
}

t_result OspfHandlerMarben::disableInterface(const in_addr ip) {
    CCLOG_ENTER_FUNCTION(XcpLogger::MOD_OSPF);
    t_result ret = OSPF_SUCCESS;

    // >>>>>
    // Sanity checks
    // >>>>>
    if (mp_GmplsControllers == NULL) {
        ret = ERROR_OSPF_MARBEN_CONTROLLERS_NOT_SET;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }

    if (!mp_GmplsControllers->isStackStarted()) {
        ret = ERROR_OSPF_INVALID_STATE;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }
    // >>>>>
    // End of sanity checks
    // >>>>>

    ret = gmpls_ospf_disable_interface(ip);

    CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
    return ret;
}

t_result OspfHandlerMarben::getInterface(const in_addr ip,
    OspfInterface& interface) {
    CCLOG_ENTER_FUNCTION(XcpLogger::MOD_OSPF);
    t_result ret = OSPF_SUCCESS;

    OspfInterfaceCtx interfaceCtx = NULL;

    // >>>>>
    // Sanity checks
    // >>>>>
    if (mp_GmplsControllers == NULL) {
        ret = ERROR_OSPF_MARBEN_CONTROLLERS_NOT_SET;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }

    if (!mp_GmplsControllers->isStackStarted()) {
        ret = ERROR_OSPF_INVALID_STATE;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }
    // >>>>>
    // End of sanity checks
    // >>>>>

    ret = gmpls_ospf_get_interface(ip, &interfaceCtx);
    if (ret == GMPLS_OSPF_RESOURCE_SHORTAGE) {
        // There's nothing we could do.
        gmpls_ospf_destroy_interface_context(&interfaceCtx);
        ret = ERROR_OSPF_RESOURCE_SHORTAGE;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    } else if (ret == GMPLS_OSPF_NOT_FOUND) {
        // @todo Log something when the OSPF-TE interface being enabled was
        // not found
        ret = ERROR_OSPF_NOT_FOUND;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    } else if (ret == GMPLS_OSPF_INVALID_INTERFACE) {
        // @todo Log something when the OSPF-TE interface being enabled was
        // not yet configured
        ret = ERROR_OSPF_INVALID_INTERFACE;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }

    ospf_read_interface_from_context(interfaceCtx, interface);

    ret = gmpls_ospf_destroy_interface_context(&interfaceCtx);

    CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
    return ret;
}

t_result OspfHandlerMarben::getInterfaces(
    std::vector<OspfInterface>& interfaces) {
    CCLOG_ENTER_FUNCTION(XcpLogger::MOD_OSPF);
    t_result ret = OSPF_SUCCESS;

    OspfInterfaceCtx interfaceCtx = NULL;
    OspfInterface currInterface;
    t_list interfaceList;
    t_list_elem * listElem = NULL;

    interfaces.clear();

    // >>>>>
    // Sanity checks
    // >>>>>
    if (mp_GmplsControllers == NULL) {
        ret = ERROR_OSPF_MARBEN_CONTROLLERS_NOT_SET;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }

    if (!mp_GmplsControllers->isStackStarted()) {
        ret = ERROR_OSPF_INVALID_STATE;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }
    // >>>>>
    // End of sanity checks
    // >>>>>

    list_init_list(&interfaceList);

    ret = gmpls_ospf_get_interfaces(&interfaceList);

    if (ret == GMPLS_OSPF_SUCCESS) {

        listElem = list_get_elem_front(&interfaceList);
        while (listElem != NULL) {
            interfaceCtx = list_get_object(listElem);
            ospf_read_interface_from_context(interfaceCtx, currInterface);
            interfaces.push_back(currInterface);
            gmpls_ospf_destroy_interface_context(&interfaceCtx);
            listElem = list_get_next_elem(listElem);
        }
    }

    list_force_end_list(&interfaceList);

    CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
    return ret;
}

t_result OspfHandlerMarben::startOspf() {
    CCLOG_ENTER_FUNCTION(XcpLogger::MOD_OSPF);
    t_result ret = OSPF_SUCCESS;

    // >>>>>
    // Sanity checks
    // >>>>>
    if (mp_GmplsControllers == NULL) {
        ret = ERROR_OSPF_MARBEN_CONTROLLERS_NOT_SET;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }

    if (!mp_GmplsControllers->isStackStarted()) {
        ret = ERROR_OSPF_INVALID_STATE;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }
    // >>>>>
    // End of sanity checks
    // >>>>>

    ret = gmpls_ospf_start();
    if (ret == GMPLS_OSPF_SUCCESS) {
        // Any other condition doesn't start OSPF-TE
        m_IsOspfStarted = 1;
    }

    CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
    return ret;
}

t_result OspfHandlerMarben::stopOspf() {
    CCLOG_ENTER_FUNCTION(XcpLogger::MOD_OSPF);
    t_result ret = OSPF_SUCCESS;

    // >>>>>
    // Sanity checks
    // >>>>>
    if (mp_GmplsControllers == NULL) {
        ret = ERROR_OSPF_MARBEN_CONTROLLERS_NOT_SET;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }

    if (!mp_GmplsControllers->isStackStarted()) {
        ret = ERROR_OSPF_INVALID_STATE;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }
    // >>>>>
    // End of sanity checks
    // >>>>>

    ret = gmpls_ospf_stop();
    if (ret == GMPLS_OSPF_SUCCESS) {
        // Any other condition doesn't stop OSPF-TE
        m_IsOspfStarted = 0;
    }

    CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
    return ret;
}

t_result OspfHandlerMarben::getNeighbors(std::vector<OspfNeighbor>& neighbors) {
    CCLOG_ENTER_FUNCTION(XcpLogger::MOD_OSPF);
    t_result ret = OSPF_SUCCESS;

    // >>>>>
    // Sanity checks
    // >>>>>

    if (mp_GmplsControllers == NULL) {
        ret = ERROR_OSPF_MARBEN_CONTROLLERS_NOT_SET;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }

    if (!mp_GmplsControllers->isStackStarted()) {
        ret = ERROR_OSPF_INVALID_STATE;
        CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
        return ret;
    }

    // >>>>>
    // End of sanity checks
    // >>>>>


    OspfNeighbor currNeighbor;
    OspfNeighborCtx currNeighborCtx = NULL;
    t_list ablNeighbors;
    t_list_elem * currElem = NULL;

    neighbors.clear();

    list_init_list(&ablNeighbors);

    ret = gmpls_ospf_get_neighbors(&ablNeighbors);
    if (ret == GMPLS_OSPF_SUCCESS) {
        currElem = list_get_elem_front(&ablNeighbors);
        while (currElem != NULL) {
            currNeighborCtx = list_get_object(currElem);

            gmpls_ospf_get_neighbor_bdr(currNeighborCtx,
                    &currNeighbor.backupDesignatedRouter);
            gmpls_ospf_get_neighbor_dr(currNeighborCtx,
                    &currNeighbor.designatedRouter);
            gmpls_ospf_get_neighbor_ip_address(currNeighborCtx,
                    &currNeighbor.ipAddress);
            gmpls_ospf_get_neighbor_router_id(currNeighborCtx,
                    &currNeighbor.routerID);
            gmpls_ospf_get_neighbor_state(currNeighborCtx, &currNeighbor.state);

            gmpls_ospf_destroy_neighbor_context(&currNeighborCtx);

            neighbors.push_back(currNeighbor);

            currElem = list_get_next_elem(currElem);
        }
    }

    list_force_end_list(&ablNeighbors);

    CCLOG_EXIT_FUNCTION(XcpLogger::MOD_OSPF, ret);
    return ret;
}

} /* namespace Xcp */
} /* namespace Cpqd */
