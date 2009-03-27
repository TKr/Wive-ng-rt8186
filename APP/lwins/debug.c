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

static const unsigned int g_loglevel = 0xffffffff;

void lwins_assert (const char *msg, const char *file, int line)
{
    fprintf (stderr, "ASSERTION FAILED: %s at %s:%d\n", msg, file, line);
    exit (1);
}

void lwins_log (int level, const char *file, int line, const char *fmt, ...)
{
    va_list args;
    
    (void)file;
    (void)line;
    
    va_start (args, fmt);
    if (!level || (g_loglevel & (1 << level)))
        vfprintf (stderr, fmt, args);
    va_end (args);
}

int lwins_check_loglevel (int level)
{
    if (!level || (g_loglevel & (1 << level)))
        return 1;
    return 0;
}

static void lwins_dump_buffer_hex (const unsigned char *buffer, size_t len)
{
    char line[(16 * 3) + 2 + 16 + 2];
    size_t i, j, n;
    char hex[3];
    
    line[sizeof (line) - 2] = '\n';
    line[sizeof (line) - 1] = '\0';
    
    i = 0;
    while (i < len)
    {
        memset (line, ' ', sizeof (line) - 2);
        n = len - i;
        if (n > 16)
            n = 16;
        
        for (j = 0; j < n; j++)
        {
            sprintf (hex, "%02X", buffer[i + j]);
            line[j * 3] = hex[0];
            line[(j * 3) + 1] = hex[1];
            line[(16 * 3) + 2 + j] = (isprint ((char)buffer[i + j]) ? buffer[i + j] : '.');
        }
        
        LOG (LOG_PACKET, "%s", line);
        i += n;
    }
}

static void lwins_dump_resource_record (const char *name, struct netbt_resource *resource, unsigned int idx)
{
    LOG (LOG_PACKET, "--- %s[%u].rname: %s\n", name, index, resource[idx].rname);
    LOG (LOG_PACKET, "--- %s[%u].rtype: %u\n", name, index, resource[idx].rtype);
    LOG (LOG_PACKET, "--- %s[%u].rclass: %u\n", name, index, resource[idx].rclass);
    LOG (LOG_PACKET, "--- %s[%u].rttl: %u\n", name, index, resource[idx].rttl);
    LOG (LOG_PACKET, "--- %s[%u].rdlen: %u\n", name, index, resource[idx].rdlen);
    lwins_dump_buffer_hex (resource[idx].rdata, resource[idx].rdlen);
}

void lwins_dump_nbns_raw_packet (unsigned char *raw_packet, size_t len, struct sockaddr_in *client)
{
    u16 tmpu16;
    struct netbt_packet packet;
    
    LOG (LOG_PACKET, "=== PACKET FROM %s:%u SIZE: %zd ===\n", inet_ntoa (client->sin_addr), ntohs(client->sin_port), len);
    
    if (netbt_parse_raw_packet (raw_packet, len, client, &packet))
    {
        LOG (LOG_PACKET, "- HEADER\n");
        LOG (LOG_PACKET, "--- tran_id: %u\n", packet.header.tran_id);
        LOG (LOG_PACKET, "--- is_response: %u\n", packet.header.is_response);
        LOG (LOG_PACKET, "--- op_code: %u\n", packet.header.op_code);
        LOG (LOG_PACKET, "--- authoritive_answer: %u\n", packet.header.authoritive_answer);
        LOG (LOG_PACKET, "--- truncation: %u\n", packet.header.truncation);
        LOG (LOG_PACKET, "--- recursion_desired: %u\n", packet.header.recursion_desired);
        LOG (LOG_PACKET, "--- recursion_available: %u\n", packet.header.recursion_available);
        LOG (LOG_PACKET, "--- broadcast: %u\n", packet.header.broadcast);
        LOG (LOG_PACKET, "--- response_code: %u\n", packet.header.response_code);
        LOG (LOG_PACKET, "--- qscnt: %u\n", packet.header.qscnt);
        LOG (LOG_PACKET, "--- ancnt: %u\n", packet.header.ancnt);
        LOG (LOG_PACKET, "--- nscnt: %u\n", packet.header.nscnt);
        LOG (LOG_PACKET, "--- arcnt: %u\n", packet.header.arcnt);
        if (packet.header.qscnt > 0)
            LOG (LOG_PACKET, "- QUESTIONS\n");
        for (tmpu16 = 0; tmpu16 < packet.header.qscnt; tmpu16++)
        {
            LOG (LOG_PACKET, "--- questions[%u].qname: %s\n", tmpu16, packet.questions[tmpu16].qname);
            LOG (LOG_PACKET, "--- questions[%u].qtype: %u\n", tmpu16, packet.questions[tmpu16].qtype);
            LOG (LOG_PACKET, "--- questions[%u].qclass: %u\n", tmpu16, packet.questions[tmpu16].qclass);
        }
        if (packet.header.ancnt > 0)
            LOG (LOG_PACKET, "- ANSWERS\n");
        for (tmpu16 = 0; tmpu16 < packet.header.ancnt; tmpu16++)
            lwins_dump_resource_record ("answers", packet.answers, tmpu16);
        if (packet.header.nscnt > 0)
            LOG (LOG_PACKET, "- AUTHORITIES\n");
        for (tmpu16 = 0; tmpu16 < packet.header.nscnt; tmpu16++)
            lwins_dump_resource_record ("authority", packet.authority, tmpu16);
        if (packet.header.arcnt > 0)
            LOG (LOG_PACKET, "- ADDITIONAL\n");
        for (tmpu16 = 0; tmpu16 < packet.header.arcnt; tmpu16++)
            lwins_dump_resource_record ("additional", packet.additional, tmpu16);
        netbt_free_packet (&packet);
    }
    else
        LOG (LOG_PACKET, "Could not parse packet\n");
    
    lwins_dump_buffer_hex (raw_packet, len);
    
    LOG (LOG_PACKET, "=== END PACKET ===\n");
}

void lwins_dump_discovery_raw_packet (unsigned char *raw_packet, size_t len, struct sockaddr_in *client)
{
    u16 tmpu16;
    struct nbns_discover_packet packet;
    
    LOG (LOG_PACKET, "=== DISCOVERY PACKET FROM %s:%u SIZE: %zd ===\n", inet_ntoa (client->sin_addr), ntohs(client->sin_port), len);
    
    memset (&packet, 0, sizeof (packet));
    if (parse_raw_nbns_discover_packet (raw_packet, len, &packet))
    {
        LOG (LOG_PACKET, "- HEADER\n");
        LOG (LOG_PACKET, "--- op_code: %u\n", packet.op_code);
        LOG (LOG_PACKET, "- ADDRESSES: %u\n", packet.addresses_cnt);
        for (tmpu16 = 0; tmpu16 < packet.addresses_cnt; tmpu16++)
            LOG (LOG_PACKET, "-- addresses[%u]: %s\n", tmpu16, inet_ntoa (packet.addresses[tmpu16]));
    }
    else
        LOG (LOG_PACKET, "Could not parse packet\n");
    
    free_nbns_discover_packet (&packet);
    
    lwins_dump_buffer_hex (raw_packet, len);
    
    LOG (LOG_PACKET, "=== END PACKET ===\n");
}

static const char *replication_msg_type (u32 msg_type)
{
    switch (msg_type)
    {
        case REPL_MSG_ASSOC_START_REQ:
            return "REPL_MSG_ASSOC_START_REQ";
        case REPL_MSG_ASSOC_START_RESP:
            return "REPL_MSG_ASSOC_START_RESP";
        case REPL_MSG_ASSOC_STOP_REQ:
            return "REPL_MSG_ASSOC_STOP_REQ";
        case REPL_MSG_PUSHPULL:
            return "REPL_MSG_PUSHPULL";
    }
    
    return "unknown";
}

void lwins_dump_replication_raw_packet (unsigned char *raw_packet, size_t len, struct sockaddr_in *client)
{
    struct replication_packet packet;
    u32 tmpu32;
    
    LOG (LOG_PACKET, "=== REPLICATION PACKET FROM %s:%u SIZE: %zd ===\n", inet_ntoa (client->sin_addr), ntohs(client->sin_port), len);
    
    memset (&packet, 0, sizeof (packet));
    
    if (replication_parse_raw_packet (raw_packet, len, &packet))
    {
        LOG (LOG_PACKET, "- HEADER\n");
        LOG (LOG_PACKET, "--- assoc_handle: %u\n", packet.header.assoc_handle);
        LOG (LOG_PACKET, "--- msg_type: %u (%s)\n", packet.header.msg_type, replication_msg_type (packet.header.msg_type));
        LOG (LOG_PACKET, "- BODY\n");
        switch (packet.header.msg_type)
        {
            case REPL_MSG_ASSOC_START_REQ:
            case REPL_MSG_ASSOC_START_RESP:
                LOG (LOG_PACKET, "--- sdr_assoc_handle: %u\n", packet.assoc_start.sdr_assoc_handle);
                LOG (LOG_PACKET, "--- nbns_version_major: %u\n", packet.assoc_start.nbns_version_major);
                LOG (LOG_PACKET, "--- nbns_version_minor: %u\n", packet.assoc_start.nbns_version_minor);
                break;
            case REPL_MSG_ASSOC_STOP_REQ:
                LOG (LOG_PACKET, "--- reason: %u\n", packet.assoc_stop.reason);
                break;
            case REPL_MSG_PUSHPULL:
                LOG (LOG_PACKET, "--- op_code: %u\n", packet.pushpull.op_code);
                switch (packet.pushpull.op_code)
                {
                    case REPL_PUSHPULL_OP_OVMAP_REQ:
                        break;
                    case REPL_PUSHPULL_OP_OVMAP_RESP:
                        LOG (LOG_PACKET, "-OWNER-VERSION MAP: %u entries\n", packet.pushpull.ovmap_resp.owners_cnt);
                        for (tmpu32 = 0; tmpu32 < packet.pushpull.ovmap_resp.owners_cnt; tmpu32++)
                        {
                            LOG (LOG_PACKET, "--- [%u] ip: %s\n", tmpu32, inet_ntoa (packet.pushpull.ovmap_resp.owners[tmpu32].addr));
                            LOG (LOG_PACKET, "--- [%u] max_version_high: %u\n", tmpu32, packet.pushpull.ovmap_resp.owners[tmpu32].max_version_high);
                            LOG (LOG_PACKET, "--- [%u] max_version_low: %u\n", tmpu32, packet.pushpull.ovmap_resp.owners[tmpu32].max_version_low);
                            LOG (LOG_PACKET, "--- [%u] min_version_high: %u\n", tmpu32, packet.pushpull.ovmap_resp.owners[tmpu32].min_version_high);
                            LOG (LOG_PACKET, "--- [%u] min_version_low: %u\n", tmpu32, packet.pushpull.ovmap_resp.owners[tmpu32].min_version_low);
                        }
                        break;
                    case REPL_PUSHPULL_OP_NREC_REQ:
                        LOG (LOG_PACKET, "--- ip: %s\n", inet_ntoa (packet.pushpull.namerec_req.rec.addr));
                        LOG (LOG_PACKET, "--- max_version_high: %u\n", packet.pushpull.namerec_req.rec.max_version_high);
                        LOG (LOG_PACKET, "--- max_version_low: %u\n", packet.pushpull.namerec_req.rec.max_version_low);
                        LOG (LOG_PACKET, "--- min_version_high: %u\n", packet.pushpull.namerec_req.rec.min_version_high);
                        LOG (LOG_PACKET, "--- min_version_low: %u\n", packet.pushpull.namerec_req.rec.min_version_low);
                        break;
                    case REPL_PUSHPULL_OP_NREC_RESP:
                        break;
                    case REPL_PUSHPOLL_OP_UN_NO_PROPAGATE:
                    case REPL_PUSHPOLL_OP_UN_PROPAGATE:
                    case REPL_PUSHPOLL_OP_UN_PERSIST_NO_PROPAGATE:
                    case REPL_PUSHPOLL_OP_UN_PERSIST_PROPAGATE:
                        LOG (LOG_PACKET, "--- initiator_addr: %s\n", inet_ntoa (packet.pushpull.updatenotify.initiator_addr));
                        LOG (LOG_PACKET, "-UPDATE-NOTIFICATION: %u entries\n", packet.pushpull.updatenotify.owners_cnt);
                        for (tmpu32 = 0; tmpu32 < packet.pushpull.updatenotify.owners_cnt; tmpu32++)
                        {
                            LOG (LOG_PACKET, "--- [%u] ip: %s\n", tmpu32, inet_ntoa (packet.pushpull.updatenotify.owners[tmpu32].addr));
                            LOG (LOG_PACKET, "--- [%u] max_version_high: %u\n", tmpu32, packet.pushpull.updatenotify.owners[tmpu32].max_version_high);
                            LOG (LOG_PACKET, "--- [%u] max_version_low: %u\n", tmpu32, packet.pushpull.updatenotify.owners[tmpu32].max_version_low);
                            LOG (LOG_PACKET, "--- [%u] min_version_high: %u\n", tmpu32, packet.pushpull.updatenotify.owners[tmpu32].min_version_high);
                            LOG (LOG_PACKET, "--- [%u] min_version_low: %u\n", tmpu32, packet.pushpull.updatenotify.owners[tmpu32].min_version_low);
                        }
                        break;
                    default:
                        LOG (LOG_PACKET, "--- Unknown op_code!\n");
                        break;
                }
                break;
        }
    }
    replication_free_packet (&packet);
    
    lwins_dump_buffer_hex (raw_packet, len);
    
    LOG (LOG_PACKET, "=== END PACKET ===\n");
}

