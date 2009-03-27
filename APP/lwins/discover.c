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

#define DISCOVER_SIGID 0x0000ABCD
#define DISCOVER_SIGID_MIN DISCOVER_SIGID
#define DISCOVER_SIGID_MAX (DISCOVER_SIGID + 2)

void free_nbns_discover_packet (struct nbns_discover_packet *packet)
{
    if (packet->addresses)
        lwins_free (packet->addresses);
    memset (packet, 0, sizeof (*packet));
}

size_t calculate_nbns_discover_packet_size (struct nbns_discover_packet *packet)
{
    return (2 + packet->addresses_cnt + 1) * sizeof (u32);
}

int parse_raw_nbns_discover_packet (unsigned char *raw_packet, size_t len, struct nbns_discover_packet *packet)
{
    u32 opcode, sigid, ip4;
    size_t i = 0;
    u16 ip4cnt, j;
    
    /* Discard packets that are too small */
    if ((size_t)len < 3 * sizeof (u32))
        return LWINS_ERR;
    
    /* SigId is little endian */
    sigid = raw_packet[i++];
    sigid |= ((u32)raw_packet[i++]) << 8;
    sigid |= ((u32)raw_packet[i++]) << 16;
    sigid |= ((u32)raw_packet[i++]) << 24;
    
    if (sigid < DISCOVER_SIGID_MIN || sigid > DISCOVER_SIGID_MAX)
        return LWINS_ERR;
    
    /* OpCode is little endian */
    opcode = raw_packet[i++];
    opcode |= ((u32)raw_packet[i++]) << 8;
    opcode |= ((u32)raw_packet[i++]) << 16;
    opcode |= ((u32)raw_packet[i++]) << 24;
    
    if (opcode > 1)
    {
        LOG (LOG_PACKET, "Unknown op code %x treated as DISCOVER_OP_DOWN!\n", opcode);
        opcode = DISCOVER_OP_DOWN;
    }
    
    ip4cnt = (u16)((len - 2 * sizeof (u32)) / sizeof (u32));
    ASSERT (ip4cnt > 0);
    
    packet->addresses = lwins_alloc ((size_t)ip4cnt * sizeof (*packet->addresses));
    if (!packet->addresses)
        return LWINS_ERR;
    
    packet->op_code = opcode;
    packet->addresses_cnt = 0;
    
    j = 0;
    while (i < (2 + ip4cnt) * sizeof (u32))
    {
        memcpy (&ip4, &raw_packet[i], sizeof (u32));
        if (!ip4)
            break;
        packet->addresses[j++].s_addr = (unsigned long)ip4;
        i += sizeof (u32);
    }
    
    packet->addresses_cnt = j;
    
    return 1;
}

int build_raw_nbns_discover_packet (unsigned char *raw_packet, size_t len, struct nbns_discover_packet *packet)
{
    static const u32 sigid = DISCOVER_SIGID;
    u32 ip4;
    u16 j;
    size_t i = 0;
    
    ASSERT ((size_t)len >= 3 * sizeof (u32));
    if ((size_t)len < (2 + packet->addresses_cnt + 1) * sizeof (u32))
        return LWINS_ERR;
    
    /* SigId is little endian */
    raw_packet[i++] = (unsigned char)(sigid & 0xFF);
    raw_packet[i++] = (unsigned char)((sigid >> 8) & 0xFF);
    raw_packet[i++] = (unsigned char)((sigid >> 16) & 0xFF);
    raw_packet[i++] = (unsigned char)((sigid >> 24) & 0xFF);
    
    /* OpCode is little endian */
    raw_packet[i++] = (unsigned char)(packet->op_code & 0xFF);
    raw_packet[i++] = (unsigned char)((packet->op_code >> 8) & 0xFF);
    raw_packet[i++] = (unsigned char)((packet->op_code >> 16) & 0xFF);
    raw_packet[i++] = (unsigned char)((packet->op_code >> 24) & 0xFF);
    
    for (j = 0; j < packet->addresses_cnt; j++)
    {
        ip4 = (u32)packet->addresses[j].s_addr;
        memcpy (&raw_packet[i], &ip4, sizeof (u32));
        i += sizeof (u32);
    }
    
    /* Add 0.0.0.0 to indicate list termination */
    ip4 = 0;
    memcpy (&raw_packet[i], &ip4, sizeof (u32));
    i += sizeof (u32);
    
    return i;
}

