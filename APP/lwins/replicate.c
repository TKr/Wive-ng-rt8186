/*
    LWINS - Lightweight WINS Server http://www.lwins.org/

    Copyright (c) 2008 by Thomas Bluemel <thomasb@lwins.org>

    This file is part of LWINS.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */
#include <lwins.h>

static int replication_build_raw_packet_header (unsigned char *raw_packet, size_t packet_size, struct replication_packet_header *header)
{
    u32 tmpu32;
    
    if (packet_size < 4 * sizeof (u32))
        return LWINS_ERR;
    
    ASSERT (header->msg_type <= REPL_MSG_MAX);
    
    memset (raw_packet, 0, 4 * sizeof (u32));
    raw_packet[sizeof (u32) + sizeof (u16)] = 0x78;
    
    tmpu32 = htonl (header->assoc_handle);
    memcpy (&raw_packet[2 * sizeof (u32)], &tmpu32, sizeof (u32));
    tmpu32 = htonl (header->msg_type);
    memcpy (&raw_packet[3 * sizeof (u32)], &tmpu32, sizeof (u32));
    return 1;
}

static int replication_parse_raw_packet_header (unsigned char *raw_packet, size_t packet_size, struct replication_packet_header *header)
{
    u8 sig_id;
    u32 tmpu32;
    
    if (packet_size < 4 * sizeof (u32))
        return LWINS_ERR;
    
    sig_id = raw_packet[sizeof (u32) + sizeof (u16)];
    if ((sig_id & 0x78) != 0x78)
        return LWINS_ERR;
    
    memcpy (&tmpu32, &raw_packet[2 * sizeof (u32)], sizeof (u32));
    header->assoc_handle = ntohl (tmpu32);
    memcpy (&tmpu32, &raw_packet[3 * sizeof (u32)], sizeof (u32));
    header->msg_type = ntohl (tmpu32);
    if (header->msg_type > REPL_MSG_MAX)
        return LWINS_ERR;
    return 1;
}

static int replication_build_raw_assoc_start (unsigned char *raw_packet, size_t packet_size, struct replication_assoc_start *start)
{
    u32 tmpu32;
    u16 tmpu16;
    
    if (packet_size != sizeof (u32) + 2 * sizeof (u16) + 21)
        return LWINS_ERR;
    
    tmpu32 = htonl (start->sdr_assoc_handle);
    memcpy (raw_packet, &tmpu32, sizeof (u32));
    tmpu16 = htons (start->nbns_version_major);
    memcpy (&raw_packet[sizeof (u32)], &tmpu16, sizeof (u16));
    tmpu16 = htons (start->nbns_version_minor);
    memcpy (&raw_packet[sizeof (u32) + sizeof (u16)], &tmpu16, sizeof (u16));
    memset (&raw_packet[2 * sizeof (u32)], 0, 21);
    return 1;
}

static int replication_parse_raw_assoc_start (unsigned char *raw_packet, size_t packet_size, struct replication_assoc_start *start)
{
    u32 tmpu32;
    u16 tmpu16;
    
    if (packet_size != sizeof (u32) + 2 * sizeof (u16) + 21)
        return LWINS_ERR;
    
    memcpy (&tmpu32, raw_packet, sizeof (u32));
    start->sdr_assoc_handle = ntohl (tmpu32);
    memcpy (&tmpu16, &raw_packet[sizeof (u32)], sizeof (u16));
    start->nbns_version_major = ntohs (tmpu16);
    memcpy (&tmpu16, &raw_packet[sizeof (u32) + sizeof (u16)], sizeof (u16));
    start->nbns_version_minor = ntohs (tmpu16);
    return 1;
}

static int replication_build_raw_assoc_stop (unsigned char *raw_packet, size_t packet_size, struct replication_assoc_stop *stop)
{
    u32 tmpu32;
    
    if (packet_size != sizeof (u32) + 25)
        return LWINS_ERR;
    
    tmpu32 = htonl (stop->reason);
    memcpy (raw_packet, &tmpu32, sizeof (u32));
    memset (&raw_packet[sizeof (u32)], 0, 25);
    return 1;
}

static int replication_parse_raw_assoc_stop (unsigned char *raw_packet, size_t packet_size, struct replication_assoc_stop *stop)
{
    u32 tmpu32;
    
    if (packet_size != sizeof (u32) + 25)
        return LWINS_ERR;
    
    memcpy (&tmpu32, raw_packet, sizeof (u32));
    stop->reason = ntohl (tmpu32);
    return 1;
}

static void replication_build_owner_record (unsigned char *raw_packet, struct replication_owner_record *rec)
{
    u32 tmpu32;
    
    tmpu32 = (u32)rec->addr.s_addr;
    memcpy (raw_packet, &tmpu32, sizeof (u32));
    tmpu32 = htonl (rec->max_version_high);
    memcpy (&raw_packet[sizeof (u32)], &tmpu32, sizeof (u32));
    tmpu32 = htonl (rec->max_version_low);
    memcpy (&raw_packet[2 * sizeof (u32)], &tmpu32, sizeof (u32));
    tmpu32 = htonl (rec->min_version_high);
    memcpy (&raw_packet[3 * sizeof (u32)], &tmpu32, sizeof (u32));
    tmpu32 = htonl (rec->min_version_low);
    memcpy (&raw_packet[4 * sizeof (u32)], &tmpu32, sizeof (u32));
    tmpu32 = htonl (1);
    memcpy (&raw_packet[5 * sizeof (u32)], &tmpu32, sizeof (u32));
}

static void replication_parse_owner_record (unsigned char *raw_packet, struct replication_owner_record *rec)
{
    u32 tmpu32;
    
    memcpy (&tmpu32, raw_packet, sizeof (u32));
    rec->addr.s_addr = tmpu32;
    
    memcpy (&tmpu32, &raw_packet[sizeof (u32)], sizeof (u32));
    rec->max_version_high = ntohl (tmpu32);
    memcpy (&tmpu32, &raw_packet[2 * sizeof (u32)], sizeof (u32));
    rec->max_version_low = ntohl (tmpu32);
    memcpy (&tmpu32, &raw_packet[sizeof (u32)], sizeof (u32));
    rec->min_version_high = ntohl (tmpu32);
    memcpy (&tmpu32, &raw_packet[2 * sizeof (u32)], sizeof (u32));
    rec->min_version_low = ntohl (tmpu32);
}

static size_t replication_build_owner_records (unsigned char *raw_packet, size_t packet_size, u32 owners_cnt, struct replication_owner_record *owners)
{
    u32 cnt, i;
    size_t size;
    
    if (packet_size < sizeof (u32))
        return LWINS_ERR;
    
    size = sizeof (u32);
    cnt = htonl (owners_cnt);
    memcpy (raw_packet, &cnt, sizeof (u32));
    
    if (owners_cnt > 0)
    {
        if (packet_size < sizeof (u32) + ((size_t)owners_cnt * 6 * sizeof (u32)))
            return LWINS_ERR;
        
        for (i = 0; i < owners_cnt; i++)
        {
            replication_build_owner_record (&raw_packet[size], &owners[i]);
            size += 6 * sizeof (u32);
        }
    }
    
    return size;
}

static size_t replication_parse_owner_records (unsigned char *raw_packet, size_t packet_size, u32 *owners_cnt, struct replication_owner_record **owners_list)
{
    u32 cnt, i;
    size_t size;
    struct replication_owner_record *owners = NULL;
    
    if (packet_size < sizeof (u32))
        return LWINS_ERR;
    
    size = sizeof (u32);
    memcpy (&cnt, raw_packet, sizeof (u32));
    cnt = ntohl (cnt);
    
    if (cnt > 0)
    {
        if (packet_size < sizeof (u32) + ((size_t)cnt * 6 * sizeof (u32)))
            return LWINS_ERR;
        
        owners = lwins_alloc ((size_t)cnt * sizeof (*owners));
        if (!owners)
            return LWINS_ERR;
        
        for (i = 0; i < cnt; i++)
        {
            replication_parse_owner_record (&raw_packet[size], &owners[i]);
            size += 6 * sizeof (u32);
        }
    }
    
    *owners_cnt = cnt;
    *owners_list = owners;
    return size;
}

static int replication_build_raw_pushpull_ovmap (unsigned char *raw_packet, size_t packet_size, struct replication_pushpull_ovmap *ovmap)
{
    size_t size;
    
    if (packet_size < 2 * sizeof (u32))
        return LWINS_ERR;
    
    size = replication_build_owner_records (raw_packet, packet_size, ovmap->owners_cnt, ovmap->owners);
    if (!size || packet_size - size != sizeof (u32))
        return LWINS_ERR;
    
    return 1;
}

static int replication_parse_raw_pushpull_ovmap (unsigned char *raw_packet, size_t packet_size, struct replication_pushpull_ovmap *ovmap)
{
    size_t size;
    
    if (packet_size < 2 * sizeof (u32))
        return LWINS_ERR;
    
    size = replication_parse_owner_records (raw_packet, packet_size, &ovmap->owners_cnt, &ovmap->owners);
    if (!size || packet_size - size != sizeof (u32))
        return LWINS_ERR;
    
    return 1;
}

static int replication_build_raw_update_notification (unsigned char *raw_packet, size_t packet_size, struct replication_pushpull_updatenotify *updatenotify)
{
    u32 tmpu32;
    size_t size;
    
    if (packet_size < 2 * sizeof (u32))
        return LWINS_ERR;
    
    size = replication_build_owner_records (raw_packet, packet_size, updatenotify->owners_cnt, updatenotify->owners);
    if (!size || packet_size - size != sizeof (u32))
        return LWINS_ERR;
    
    tmpu32 = (u32)updatenotify->initiator_addr.s_addr;
    memcpy (&raw_packet[size], &tmpu32, sizeof (u32));
    return 1;
}

static int replication_parse_raw_update_notification (unsigned char *raw_packet, size_t packet_size, struct replication_pushpull_updatenotify *updatenotify)
{
    u32 tmpu32;
    size_t size;
    
    if (packet_size < 2 * sizeof (u32))
        return LWINS_ERR;
    
    size = replication_parse_owner_records (raw_packet, packet_size, &updatenotify->owners_cnt, &updatenotify->owners);
    if (!size || packet_size - size != sizeof (u32))
        return LWINS_ERR;
    
    memcpy (&tmpu32, &raw_packet[size], sizeof (u32));
    updatenotify->initiator_addr.s_addr = tmpu32;
    return 1;
}

static int replication_build_raw_pushpull_namerec_req (unsigned char *raw_packet, size_t packet_size, struct replication_pushpull_namerec_req *req)
{
    if (packet_size != 6 * sizeof (u32))
        return LWINS_ERR;
    
    replication_build_owner_record (raw_packet, &req->rec);
    return 1;
}

static int replication_parse_raw_pushpull_namerec_req (unsigned char *raw_packet, size_t packet_size, struct replication_pushpull_namerec_req *req)
{
    if (packet_size != 6 * sizeof (u32))
        return LWINS_ERR;
    
    replication_parse_owner_record (raw_packet, &req->rec);
    return 1;
}

static size_t replication_build_raw_name_record (unsigned char *raw_packet, size_t packet_size, struct replication_name_record *rec)
{
    u32 tmpu32;
    u8 flags, j;
    size_t size;
    
    if (packet_size < 6 * sizeof (u32))
        return LWINS_ERR;
    
    /* netbios name */
    ASSERT (rec->name);
    tmpu32 = strlen (rec->name);
    ASSERT (tmpu32 > 0 && tmpu32 <= 0xFF);
    tmpu32 = htonl (tmpu32) + 1;
    
    if (packet_size < sizeof (u32) + tmpu32)
        return LWINS_ERR;
    
    memcpy (raw_packet, &tmpu32, sizeof (u32));
    tmpu32 = (tmpu32 + 3) & ~3;
    if (packet_size < sizeof (u32) + tmpu32 + 5 * sizeof (u32))
        return LWINS_ERR;
    
    memset (&raw_packet[sizeof (u32)], 0, tmpu32 + 3);
    strcpy ((char *)&raw_packet[sizeof (u32)], rec->name);
    
    size = tmpu32 + sizeof (u32);
    raw_packet += size;
    packet_size -= size;
    
    /* flags */
    ASSERT (rec->is_static <= 1);
    ASSERT (rec->node_type <= 3);
    ASSERT (rec->is_replica <= 1);
    ASSERT (rec->state <= 3);
    ASSERT (rec->type <= 3);
    flags = rec->is_static;
    flags |= rec->node_type << 1;
    flags |= rec->is_replica << 3;
    flags |= rec->state << 4;
    flags |= rec->type << 6;
    raw_packet[sizeof (u32) - 1] = flags;
    
    /* group */
    raw_packet[sizeof (u32)] = (rec->type == NREC_TYPE_GROUP || rec->type == NREC_TYPE_GROUP_SPECIAL);
    
    /* version */
    tmpu32 = htonl (rec->version_high);
    memcpy (&raw_packet[2 * sizeof (u32)], &tmpu32, sizeof (u32));
    tmpu32 = htonl (rec->version_low);
    memcpy (&raw_packet[3 * sizeof (u32)], &tmpu32, sizeof (u32));
    
    size += 4 * sizeof (u32);
    raw_packet += 4 * sizeof (u32);
    packet_size -= 4 * sizeof (u32);
    
    /* address record */
    raw_packet[0] = rec->addrs_cnt;
    memset (&raw_packet[1], 0, sizeof (u32) - 1);
    if (packet_size < sizeof (u32) + ((size_t)rec->addrs_cnt * 2 * sizeof (u32)) + sizeof (u32))
        return LWINS_ERR;
    
    size += sizeof (u32) + ((size_t)rec->addrs_cnt * 2 * sizeof (u32));
    raw_packet += sizeof (u32);
    for (j = 0; j < rec->addrs_cnt; j++)
    {
        tmpu32 = (u32)rec->addrs[j].owner_addr.s_addr;
        memcpy (raw_packet, &tmpu32, sizeof (u32));
        tmpu32 = (u32)rec->addrs[j].member_addr.s_addr;
        memcpy (&raw_packet[sizeof (u32)], &tmpu32, sizeof (u32));
        
        raw_packet += 2 * sizeof (u32);
    }
    
    /* reserved */
    tmpu32 = htonl (0xFFFFFFFF);
    memcpy (raw_packet, &tmpu32, sizeof (u32));
    return size + sizeof (u32);
}

static size_t replication_parse_raw_name_record (unsigned char *raw_packet, size_t packet_size, struct replication_name_record *rec)
{
    u32 tmpu32;
    u8 flags, group, j;
    size_t size;
    
    if (packet_size < 6 * sizeof (u32))
        return LWINS_ERR;
    
    /* netbios name */
    memcpy (&tmpu32, raw_packet, sizeof (u32));
    tmpu32 = ntohl (tmpu32);
    if (tmpu32 == 0 || tmpu32 > 0xFF)
        return LWINS_ERR;
    if (packet_size < sizeof (u32) + tmpu32)
        return LWINS_ERR;
    rec->name = lwins_alloc (tmpu32);
    if (!rec->name)
        return LWINS_ERR;
    memcpy (rec->name, &raw_packet[sizeof (u32)], tmpu32);
    if (rec->name[tmpu32 - 1] != '\0' ||
        strlen (rec->name) < tmpu32 - 1)
        return LWINS_ERR;
    
    /* 4 byte alignment */
    tmpu32 = (tmpu32 + 3) & ~3;
    if (packet_size < sizeof (u32) + tmpu32 + 5 * sizeof (u32))
        return LWINS_ERR;
    
    size = tmpu32 + sizeof (u32);
    raw_packet += size;
    packet_size -= size;
    
    /* flags */
    flags = raw_packet[sizeof (u32) - 1];
    if (flags & (1 << 0))
        rec->is_static = 1;
    rec->node_type = (flags >> 1) & 0x3;
    if (flags & (1 << 3))
        rec->is_replica = 1;
    rec->state = (flags >> 4) & 0x3;
    rec->type = (flags >> 6) & 0x3;
    
    /* group */
    group = raw_packet[sizeof (u32)];
    if ((rec->type == NREC_TYPE_UNIQUE || rec->type == NREC_TYPE_MULTIHOMED) && group != 0)
        return LWINS_ERR;
    if ((rec->type == NREC_TYPE_GROUP || rec->type == NREC_TYPE_GROUP_SPECIAL) && group == 0)
        return LWINS_ERR;
    
    /* version */
    memcpy (&tmpu32, &raw_packet[2 * sizeof (u32)], sizeof (u32));
    rec->version_high = ntohl (tmpu32);
    memcpy (&tmpu32, &raw_packet[3 * sizeof (u32)], sizeof (u32));
    rec->version_low = ntohl (tmpu32);
    
    size += 4 * sizeof (u32);
    raw_packet += 4 * sizeof (u32);
    packet_size -= 4 * sizeof (u32);
    
    /* address record */
    rec->addrs_cnt = raw_packet[0];
    if (packet_size < sizeof (u32) + ((size_t)rec->addrs_cnt * 2 * sizeof (u32)) + sizeof (u32))
        return LWINS_ERR;
    rec->addrs = lwins_alloc ((size_t)rec->addrs_cnt * sizeof (*rec->addrs));
    if (!rec->addrs)
        return LWINS_ERR;
    
    size += sizeof (u32) + ((size_t)rec->addrs_cnt * 2 * sizeof (u32));
    raw_packet += sizeof (u32);
    for (j = 0; j < rec->addrs_cnt; j++)
    {
        memcpy (&tmpu32, raw_packet, sizeof (u32));
        rec->addrs[j].owner_addr.s_addr = tmpu32;
        memcpy (&tmpu32, &raw_packet[sizeof (u32)], sizeof (u32));
        rec->addrs[j].member_addr.s_addr = tmpu32;
        
        raw_packet += 2 * sizeof (u32);
    }
    
    /* reserved field */
    return size + sizeof (u32);
}

static int replication_build_raw_pushpull_namerec_resp (unsigned char *raw_packet, size_t packet_size, struct replication_pushpull_namerec_resp *resp)
{
    u32 tmpu32, i;
    size_t size;
    
    if (packet_size < sizeof (u32))
        return LWINS_ERR;
    
    tmpu32 = htonl (resp->namerecs_cnt);
    memcpy (raw_packet, &tmpu32, sizeof (u32));
    raw_packet += sizeof (u32);
    packet_size -= sizeof (u32);
    
    for (i = 0; i < resp->namerecs_cnt; i++)
    {
        ASSERT (resp->namerecs[i]);
        size = replication_build_raw_name_record (raw_packet, packet_size, resp->namerecs[i]);
        ASSERT (size >= packet_size);
        packet_size -= size;
        raw_packet += size;
    }
    
    if (packet_size != 0)
        return LWINS_ERR;
    return 1;
}

static int replication_parse_raw_pushpull_namerec_resp (unsigned char *raw_packet, size_t packet_size, struct replication_pushpull_namerec_resp *resp)
{
    u32 tmpu32, i;
    size_t size;
    
    if (packet_size < sizeof (u32))
        return LWINS_ERR;
    
    memcpy (&tmpu32, raw_packet, sizeof (u32));
    resp->namerecs_cnt = ntohl (tmpu32);
    raw_packet += sizeof (u32);
    packet_size -= sizeof (u32);
    
    if (resp->namerecs_cnt > 0)
    {
        resp->namerecs = lwins_alloc (resp->namerecs_cnt * sizeof (*resp->namerecs));
        if (!resp->namerecs)
            return LWINS_ERR;
        memset (resp->namerecs, 0, resp->namerecs_cnt * sizeof (*resp->namerecs));
        
        for (i = 0; i < resp->namerecs_cnt; i++)
        {
            resp->namerecs[i] = lwins_alloc (sizeof (struct replication_name_record));
            if (!resp->namerecs[i])
                return LWINS_ERR;
            memset (resp->namerecs[i], 0, sizeof (struct replication_name_record));
            
            size = replication_parse_raw_name_record (raw_packet, packet_size, resp->namerecs[i]);
            packet_size -= size;
            raw_packet += size;
        }
    }
    
    if (packet_size != 0)
        return LWINS_ERR;
    return 1;
}

static int replication_build_raw_pushpull (unsigned char *raw_packet, size_t packet_size, struct replication_pushpull *pushpull)
{
    if (packet_size < sizeof (u32))
        return LWINS_ERR;
    
    memset (raw_packet, 0, 3);
    raw_packet[3] = pushpull->op_code;
    raw_packet += sizeof (u32);
    packet_size -= sizeof (u32);
    switch (pushpull->op_code)
    {
        case REPL_PUSHPULL_OP_OVMAP_REQ:
            if (packet_size != 0)
                return LWINS_ERR;
            return 1;
        
        case REPL_PUSHPULL_OP_OVMAP_RESP:
            return replication_build_raw_pushpull_ovmap (raw_packet, packet_size, &pushpull->ovmap_resp);
        
        case REPL_PUSHPULL_OP_NREC_REQ:
            return replication_build_raw_pushpull_namerec_req (raw_packet, packet_size, &pushpull->namerec_req);
        
        case REPL_PUSHPULL_OP_NREC_RESP:
            return replication_build_raw_pushpull_namerec_resp (raw_packet, packet_size, &pushpull->namerec_resp);
        
        case REPL_PUSHPOLL_OP_UN_NO_PROPAGATE:
        case REPL_PUSHPOLL_OP_UN_PROPAGATE:
        case REPL_PUSHPOLL_OP_UN_PERSIST_NO_PROPAGATE:
        case REPL_PUSHPOLL_OP_UN_PERSIST_PROPAGATE:
            return replication_build_raw_update_notification (raw_packet, packet_size, &pushpull->updatenotify);
    }
    
    return LWINS_ERR;
}

static int replication_parse_raw_pushpull (unsigned char *raw_packet, size_t packet_size, struct replication_pushpull *pushpull)
{
    if (packet_size < sizeof (u32))
        return LWINS_ERR;
    
    pushpull->op_code = raw_packet[3];
    raw_packet += sizeof (u32);
    packet_size -= sizeof (u32);
    switch (pushpull->op_code)
    {
        case REPL_PUSHPULL_OP_OVMAP_REQ:
            if (packet_size != 0)
                return LWINS_ERR;
            return 1;
        
        case REPL_PUSHPULL_OP_OVMAP_RESP:
            return replication_parse_raw_pushpull_ovmap (raw_packet, packet_size, &pushpull->ovmap_resp);
        
        case REPL_PUSHPULL_OP_NREC_REQ:
            return replication_parse_raw_pushpull_namerec_req (raw_packet, packet_size, &pushpull->namerec_req);
        
        case REPL_PUSHPULL_OP_NREC_RESP:
            return replication_parse_raw_pushpull_namerec_resp (raw_packet, packet_size, &pushpull->namerec_resp);
        
        case REPL_PUSHPOLL_OP_UN_NO_PROPAGATE:
        case REPL_PUSHPOLL_OP_UN_PROPAGATE:
        case REPL_PUSHPOLL_OP_UN_PERSIST_NO_PROPAGATE:
        case REPL_PUSHPOLL_OP_UN_PERSIST_PROPAGATE:
            return replication_parse_raw_update_notification (raw_packet, packet_size, &pushpull->updatenotify);
    }
    
    return LWINS_ERR;
}

int replication_build_raw_packet (unsigned char *raw_packet, size_t packet_size, struct replication_packet *packet)
{
    if (!replication_build_raw_packet_header (raw_packet, packet_size, &packet->header))
        return LWINS_ERR;
    
    raw_packet += 4 * sizeof (u32);
    packet_size -= 4 * sizeof (u32);
    switch (packet->header.msg_type)
    {
        case REPL_MSG_ASSOC_START_REQ:
        case REPL_MSG_ASSOC_START_RESP:
            return replication_build_raw_assoc_start (raw_packet, packet_size, &packet->assoc_start);
        
        case REPL_MSG_ASSOC_STOP_REQ:
            return replication_build_raw_assoc_stop (raw_packet, packet_size, &packet->assoc_stop);
        
        case REPL_MSG_PUSHPULL:
            return replication_build_raw_pushpull (raw_packet, packet_size, &packet->pushpull);
    }
    
    return LWINS_ERR;
}

int replication_parse_raw_packet (unsigned char *raw_packet, size_t packet_size, struct replication_packet *packet)
{
    if (!replication_parse_raw_packet_header (raw_packet, packet_size, &packet->header))
        return LWINS_ERR;
    
    raw_packet += 4 * sizeof (u32);
    packet_size -= 4 * sizeof (u32);
    switch (packet->header.msg_type)
    {
        case REPL_MSG_ASSOC_START_REQ:
        case REPL_MSG_ASSOC_START_RESP:
            /* FIXME: Need to check which of those messages is valid in this context */
            return replication_parse_raw_assoc_start (raw_packet, packet_size, &packet->assoc_start);
        
        case REPL_MSG_ASSOC_STOP_REQ:
            return replication_parse_raw_assoc_stop (raw_packet, packet_size, &packet->assoc_stop);
        
        case REPL_MSG_PUSHPULL:
            return replication_parse_raw_pushpull (raw_packet, packet_size, &packet->pushpull);
    }
    
    return LWINS_ERR;
}

static size_t replication_calc_raw_packet_size (struct replication_packet *packet)
{
    size_t size;
    
    /* header */
    size = 4 * sizeof (u32);
    
    switch (packet->header.msg_type)
    {
        case REPL_MSG_ASSOC_START_REQ:
        case REPL_MSG_ASSOC_START_RESP:
            size += 2 * sizeof (u32) + 21;
            break;
        
        case REPL_MSG_ASSOC_STOP_REQ:
            size += sizeof (u32) + 25;
            break;
        
        case REPL_MSG_PUSHPULL:
            ASSERT (0); /* FIXME: Implement */
            break;
        
        default:
            return LWINS_ERR;
    }
    
    return size;
}

static void replication_free_namerec (struct replication_name_record *rec)
{
    if (rec->name)
        lwins_free (rec->name);
    if (rec->addrs)
        lwins_free (rec->addrs);
    lwins_free (rec);
}

void replication_free_packet (struct replication_packet *packet)
{
    if (packet->header.msg_type == REPL_MSG_PUSHPULL)
    {
        switch (packet->pushpull.op_code)
        {
            case REPL_PUSHPULL_OP_OVMAP_RESP:
                if (packet->pushpull.ovmap_resp.owners)
                    lwins_free (packet->pushpull.ovmap_resp.owners);
                break;
            case REPL_PUSHPULL_OP_NREC_RESP:
                if (packet->pushpull.namerec_resp.namerecs)
                {
                    u32 i;
                    
                    for (i = 0; i < packet->pushpull.namerec_resp.namerecs_cnt; i++)
                    {
                        if (packet->pushpull.namerec_resp.namerecs)
                            replication_free_namerec (packet->pushpull.namerec_resp.namerecs[i]);
                    }
                    
                    lwins_free (packet->pushpull.namerec_resp.namerecs);
                }
                break;
            case REPL_PUSHPOLL_OP_UN_NO_PROPAGATE:
            case REPL_PUSHPOLL_OP_UN_PROPAGATE:
            case REPL_PUSHPOLL_OP_UN_PERSIST_NO_PROPAGATE:
            case REPL_PUSHPOLL_OP_UN_PERSIST_PROPAGATE:
                if (packet->pushpull.updatenotify.owners)
                    lwins_free (packet->pushpull.updatenotify.owners);
                break;
        }
    }
    
    memset (packet, 0, sizeof (*packet));
}

struct association *alloc_association (struct replication_service *service, int sd, struct sockaddr_in *client)
{
    struct association *assoc, **association_list;
    unsigned short i;
    static const unsigned int association_table_block = 16;
    
    if ((unsigned int)service->associations_cnt + association_table_block >= 0xFFFF)
    {
        LOG (LOG_ALL, "WARNING: Maximum number of associations reached!\n");
        return NULL;
    }
    
    assoc = lwins_alloc (sizeof (*assoc));
    if (!assoc)
        return NULL;
    
    if (!service->associations)
    {
        service->associations = lwins_alloc (sizeof (service->associations[0]) * association_table_block);
        if (!service->associations)
        {
            lwins_free (assoc);
            return NULL;
        }
        
        service->associations_size = association_table_block;
        service->associations[0] = assoc;
        assoc->handle = 1;
    }
    else if (service->associations_cnt == service->associations_size)
    {        
        association_list = lwins_alloc (sizeof (service->associations[0]) * (service->associations_size + association_table_block));
        if (!association_list)
        {
            lwins_free (assoc);
            return NULL;
        }
        
        memcpy (association_list, service->associations, sizeof (service->associations[0]) * service->associations_size);
        lwins_free (service->associations);
        service->associations = association_list;
        service->associations[service->associations_size] = assoc;
        assoc->handle = (u32)service->associations_size + 1;
        service->associations_size += association_table_block;
    }
    else
    {
        for (i = 0; i < service->associations_size; i++)
        {
            if (!service->associations[i])
                break;
        }
        
        ASSERT (i < service->associations_size);
        
        service->associations[i] = assoc;
        assoc->handle = (u32)i + 1;
    }
    
    service->associations_cnt++;
    assoc->sd = sd;
    assoc->client = *client;
    
    LOG (LOG_ALL, "Established association handle %x with %s\n", assoc->handle, inet_ntoa (assoc->client.sin_addr));
    
    return assoc;
}

void free_association (struct replication_service *service, struct association *assoc)
{
    unsigned short i;
    (void)service;
    
     LOG (LOG_ALL, "Terminate association handle %x with %s\n", assoc->handle, inet_ntoa (assoc->client.sin_addr));
    
    for (i = 0; i < service->associations_size; i++)
    {
        if (service->associations[i] == assoc)
        {
            service->associations[i] = NULL;
            service->associations_cnt--;
            break;
        }
    }
    
    if (assoc->buffer)
        lwins_free (assoc->buffer);
    
    lwins_free (assoc);
}

struct association *find_association (struct replication_service *service, int sd)
{
    unsigned short i, cnt;
    
    cnt = service->associations_cnt;
    for (i = 0; i < service->associations_size; i++)
    {
        if (service->associations[i]->sd == sd)
            return service->associations[i];
        if (--cnt == 0)
            break;
    }
    
    return NULL;
}

struct replication_service *init_replication_service (struct lwins_config *cfg)
{
    struct replication_service *service;
    const char *value;
    unsigned int val;
    
    service = lwins_alloc (sizeof (*service));
    if (!service)
        return NULL;
    
    if (lwins_get_config_param (cfg, "replication-packet-size-max", 0, &value) &&
        lwins_convert_to_integer (value, &val) && val > 0)
    {
        service->max_packet_size = val * 1024;
    }
    else
        service->max_packet_size = 0;
    
    return service;
}

void free_replication_service (struct replication_service *service)
{
    unsigned short i;
    
    if (service->associations)
    {
        if (service->associations_cnt > 0)
        {
            for (i = 0; i < service->associations_size; i++)
            {
                if (service->associations[i])
                    free_association (service, service->associations[i]);
            }
        }
    }
}

static int replication_send_buffer (struct association *assoc, unsigned char *buffer, size_t size)
{
    ssize_t s;
    size_t sent = 0;
    
    do
    {
        s = send (assoc->sd, buffer + sent, size - sent, 0);
        if (s < 0)
        {
            LOGERR (LOG_ALL, "send");
            return LWINS_ERR;
        }
        
        sent += s;
    } while (sent < size);
    
    return 1;
}

static int replication_send_packet (struct replication_service *service, struct association *assoc, struct replication_packet *packet)
{
    size_t size;
    u32 tmpu32;
    int ret;
    unsigned char *raw_packet;
    
    size = replication_calc_raw_packet_size (packet);
    if (!size)
        return LWINS_ERR;
    
    if (service->max_packet_size != 0 && size + sizeof (u32) > service->max_packet_size)
    {
        LOG (LOG_ALL, "Maximum packet size %zd exceeded sending replication packet\n", service->max_packet_size);
        return LWINS_ERR;
    }
    
    raw_packet = lwins_alloc (size + sizeof (u32));
    if (!raw_packet)
        return LWINS_ERR;
    
    tmpu32 = htonl (size);
    memcpy (raw_packet, &tmpu32, sizeof (u32));
    
    if (!replication_build_raw_packet (&raw_packet[sizeof (u32)], size, packet))
    {
        lwins_free (raw_packet);
        return LWINS_ERR;
    }
    
    ret = replication_send_buffer (assoc, raw_packet, size + sizeof (u32));
    lwins_free (raw_packet);
    return ret;
}

static int replication_send_start_response (struct replication_service *service, struct association *assoc)
{
    struct replication_packet reply;
    
    reply.header.assoc_handle = assoc->sender_handle;
    reply.header.msg_type = REPL_MSG_ASSOC_START_RESP;
    reply.assoc_start.sdr_assoc_handle = assoc->handle;
    reply.assoc_start.nbns_version_major = 2;
    reply.assoc_start.nbns_version_minor = 5;
    
    return replication_send_packet (service, assoc, &reply);
}

static int replication_process_packet (struct replication_service *service, struct association *assoc, struct replication_packet *packet)
{
    (void)service;
    
    if (packet->header.msg_type != REPL_MSG_ASSOC_START_REQ && !assoc->recv_start_req)
        return LWINS_ERR;
    
    switch (packet->header.msg_type)
    {
        case REPL_MSG_ASSOC_START_REQ:
            if (assoc->recv_start_req || !packet->assoc_start.sdr_assoc_handle)
                return LWINS_ERR;
            assoc->recv_start_req = 1;
            if (packet->assoc_start.nbns_version_major != 2)
            {
                assoc->close_conn = 1;
                return 1;
            }
            
            assoc->sender_handle = packet->assoc_start.sdr_assoc_handle;
            return replication_send_start_response (service, assoc);
    }
    
    return LWINS_ERR;
}

static int association_read_raw_packet (struct replication_service *service, struct association *assoc, unsigned char *raw_packet, size_t packet_size)
{
    struct replication_packet packet;
    int ret = 0;
    
    (void)service;
    (void)assoc;
    
    memset (&packet, 0, sizeof (packet));
    if (replication_parse_raw_packet (raw_packet, packet_size, &packet))
    {
        ret = replication_process_packet (service, assoc, &packet);
        replication_free_packet (&packet);
    }
    
    return ret;
}

int association_buffer_receive(struct replication_service *service, struct association *assoc, unsigned char *buffer, size_t buffer_size)
{
    u32 packet_len;
    
    if (!assoc->buffer)
    {
        assoc->buffer = lwins_alloc (buffer_size);
        if (!assoc->buffer)
            return LWINS_ERR;
        assoc->buffer_size = buffer_size;
        assoc->buffer_length = buffer_size;
        memcpy (assoc->buffer, buffer, buffer_size);
    }
    else if (assoc->buffer_size < assoc->buffer_length + buffer_size)
    {
        unsigned char *buf;
        
        if (service->max_packet_size != 0 && assoc->buffer_length + buffer_size > service->max_packet_size)
        {
            LOG (LOG_PACKET, "Maximum replication packet size %zd exceeded!\n", service->max_packet_size);
            return LWINS_ERR;
        }
        
        buf = lwins_alloc (assoc->buffer_length + buffer_size);
        if (!buf)
            return LWINS_ERR;
        memcpy (buf, assoc->buffer, assoc->buffer_length);
        memcpy (buf + assoc->buffer_length, buffer, buffer_size);
        ASSERT (assoc->buffer != NULL);
        lwins_free (assoc->buffer);
        assoc->buffer = buf;
        assoc->buffer_length += buffer_size;
        assoc->buffer_size = assoc->buffer_length;
    }
    else
    {
        ASSERT (assoc->buffer != NULL);
        ASSERT (assoc->buffer_size >= assoc->buffer_length + buffer_size);
        
        memcpy (assoc->buffer + assoc->buffer_length, buffer, buffer_size);
        assoc->buffer_length += buffer_size;
    }
    
    while (assoc->buffer_length >= sizeof (u32))
    {
        memcpy (&packet_len, assoc->buffer, sizeof (u32));
        packet_len = ntohl (packet_len) + sizeof (u32);
        
        if (service->max_packet_size != 0 && packet_len > service->max_packet_size)
        {
            LOG (LOG_PACKET, "Maximum replication packet size %zd exceeded!\n", service->max_packet_size);
            return LWINS_ERR;
        }
        
        if (assoc->buffer_length >= packet_len)
        {
            LOGPACKET (assoc->buffer, (size_t)packet_len, &assoc->client, lwins_dump_replication_raw_packet);
            if (!association_read_raw_packet (service, assoc, assoc->buffer, packet_len))
                return LWINS_ERR;
            if (assoc->buffer_length > packet_len)
                memmove (assoc->buffer, assoc->buffer + packet_len, assoc->buffer_length - packet_len);
            assoc->buffer_length -= packet_len;
            
            if (assoc->close_conn)
                break;
        }
        else
            break;
    }
    
    return 1;
}


