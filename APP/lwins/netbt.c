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

struct netbt_packet_context
{
    unsigned char *packet;
    size_t length;
    size_t position;
    u16 labels_cnt;
    u16 labels[NETBT_MAX_LABELS];
};

#define DEFINE_NETBT_PACKET_READ(type,func) \
static __inline type netbt_packet_read_##type (struct netbt_packet_context *ctx) \
{ \
    type ret; \
    ASSERT ((size_t) ctx->position <= (size_t) ctx->length - sizeof (type)); \
    memcpy (&ret, ctx->packet + ctx->position, sizeof (type)); \
    ret = (type) func (ret); \
    ctx->position += sizeof (type); \
    return ret; \
}
#define DEFINE_NETBT_PACKET_PUT(type,func) \
static __inline void netbt_packet_put_##type (struct netbt_packet_context *ctx, type val) \
{ \
    type value; \
    value = (type) func (val); \
    ASSERT (ctx->position + sizeof (type) <= (size_t) ctx->length); \
    memcpy (ctx->packet + ctx->position, &value, sizeof (type)); \
    ctx->position += sizeof (type); \
}
DEFINE_NETBT_PACKET_READ (u8, )
DEFINE_NETBT_PACKET_PUT (u8, )
DEFINE_NETBT_PACKET_READ (u16, ntohs)
DEFINE_NETBT_PACKET_PUT (u16, htons)
DEFINE_NETBT_PACKET_READ (u32, ntohl)
DEFINE_NETBT_PACKET_PUT (u32, htonl)
#undef DEFINE_NETBT_PACKET_READ
#undef DEFINE_NETBT_PACKET_PUT

static __inline size_t netbt_packet_data_remaining (struct netbt_packet_context *ctx)
{
    if (ctx->position < ctx->length)
        return ctx->length - ctx->position;
    return LWINS_ERR;
}

static __inline void netbt_packet_write_data (struct netbt_packet_context *ctx, const unsigned char *src, size_t len)
{
    ASSERT (len > 0);
    ASSERT (ctx->position + len <= ctx->length);
    memcpy (ctx->packet + ctx->position, src, len);
    ctx->position += len;
}

static __inline void netbt_packet_read_data (struct netbt_packet_context *ctx, unsigned char *dest, size_t len)
{
    ASSERT (len > 0);
    ASSERT (ctx->position + len <= ctx->length);
    memcpy (dest, ctx->packet + ctx->position, len);
    ctx->position += len;
}

static __inline void netbt_packet_data_skip (struct netbt_packet_context *ctx, size_t skip)
{
    ASSERT (ctx->position + skip <= ctx->length);
    ctx->position += skip;
}

static __inline unsigned char * netbt_packet_current (struct netbt_packet_context *ctx)
{
    return ctx->packet + ctx->position;
}

static __inline size_t netbt_packet_position (struct netbt_packet_context *ctx)
{
    return ctx->position;
}

static __inline void netbt_set_packet_position (struct netbt_packet_context *ctx, size_t position)
{
    ASSERT (position > 0);
    ASSERT (ctx->position < ctx->length);
    ctx->position = position;
}

int is_name_reserved (const char *name)
{
    static const char *reserved_names[] = {
        "ANONYMOUS",
        "AUTHENTICATED USER",
        "BATCH",
        "BUILTIN",
        "CREATOR GROUP",
        "CREATOR GROUP SERVER",
        "CREATOR OWNER",
        "CREATOR OWNER SERVER",
        "DIALUP",
        "DIGEST AUTH",
        "INTERACTIVE",
        "INTERNET",
        "LOCAL",
        "LOCAL SYSTEM",
        "NETWORK",
        "NETWORK SERVICE",
        "NT AUTHORITY",
        "NT DOMAIN",
        "NTLM AUTH",
        "NULL",
        "PROXY",
        "REMOTE INTERACTIVE",
        "RESTRICTED",
        "SCHANNEL AUTH",
        "SELF",
        "SERVER",
        "SERVICE",
        "SYSTEM",
        "TERMINAL SERVER",
        "THIS ORGANIZATION",
        "USERS",
        "WORLD"
    };
    unsigned int i;
    
    for (i = 0; i < sizeof (ARRAY_CNT (reserved_names)); i++)
    {
        if (!strcmp (name, reserved_names[i]))
            return 1;
    }
    
    return 0;
}

static size_t extract_netbios_name (const char *name, const char **scope)
{
    size_t len, i;
    
    *scope = NULL;
    
    len = strlen (name);
    for (i = 0; i < len; i++)
    {
        if (!((name[i] >= 'a' && name[i] <= 'z') ||
              (name[i] >= 'A' && name[i] <= 'Z') ||
              (name[i] >= '0' && name[i] <= '9') ||
              name[i] == '-' || name[i] == ' '))
        {
            LOG (LOG_ALL, "Invalid netbios name: %s\n", name);
            return LWINS_ERR;
        }
        else if (name[i] == '.')
        {
            if (i > 0 && i <= NETBT_NAME_LENGTH - 1)
            {
                if (i < len - 1 && name[i + 1] != '\0')
                    *scope = &name[i + 1];
                return i;
            }
            return LWINS_ERR;
        }
        else if (i >= NETBT_NAME_LENGTH)
            return LWINS_ERR;
    }
    
    if (len <= NETBT_NAME_LENGTH)
        return len;
    
    return LWINS_ERR;
}

int is_valid_netbios_name (const char *name)
{
    int i, len;
    
    len = strlen (name);
    if (len == 0 || len > NETBT_NAME_LENGTH)
        return LWINS_ERR;
    
    for (i = 0; i < len; i++)
    {
        if (!((name[i] >= 'a' && name[i] <= 'z') ||
              (name[i] >= 'A' && name[i] <= 'Z') ||
              (name[i] >= '0' && name[i] <= '9') ||
              name[i] == '-') ||
            (i == 0 && name[i] == ' '))
        {
            LOG (LOG_ALL, "Invalid netbios name: %s\n", name);
            return LWINS_ERR;
        }
    }
    
    return 1;
}

int is_valid_domain_name (const char *name)
{
    int i, len, last_dot, label_len;
    
    len = strlen (name);
    if (len == 0 || len > 16)
        return LWINS_ERR;
    
    last_dot = -1;
    label_len = 0;
    for (i = 0; i < len; i++)
    {
        if ((name[i] >= 'a' && name[i] <= 'z') ||
            (name[i] >= 'A' && name[i] <= 'Z') ||
            (name[i] >= '0' && name[i] <= '9') ||
            name[i] == '.' || name[i] == '-')
        {
            if (name[i] == '.')
            {
                if (i == 0 || last_dot == i - 1)
                    return LWINS_ERR;
                last_dot = i;
                label_len = 0;
            }
            else if (++label_len > 63)
                return LWINS_ERR;
        }
        else
            return LWINS_ERR;
    }
    
    if (label_len == 0 && last_dot == i - 1)
        return LWINS_ERR;
    
    return 1;
}

int encode_netbios_name (const char *name, const char *scope, char *encoded, size_t encoded_size)
{
    int j, scope_len = 0;
    size_t len, i;
    char iname[16];
    
    encoded[0] = '\0';
    memset (iname, ' ', sizeof (iname));
    
    if (scope)
    {
        scope_len = strlen (scope) + 1;
        
        if (!is_valid_netbios_name (name) || (scope && !is_valid_domain_name (scope)))
            return LWINS_ERR;
        if (encoded_size <= (sizeof (iname) * 2) + scope_len)
            return LWINS_ERR;
        memcpy (iname, name, strlen (name));
    }
    else
    {
        len = extract_netbios_name (name, &scope);
        if (!len)
            return LWINS_ERR;
        if (scope)
        {
            if (!is_valid_domain_name (scope))
                return LWINS_ERR;
            scope_len = strlen (scope) + 1;
        }
        if (encoded_size <= (sizeof (iname) * 2) + scope_len)
            return LWINS_ERR;
        memcpy (iname, name, len);
    }
    
    j = 0;
    for (i = 0; i < sizeof (iname); i++)
    {
        encoded[j++] = ((iname[i] & 0xF0) >> 4) + 'A';
        encoded[j++] = (iname[i] & 0x0F) + 'A';
    }
    
    if (scope)
    {
        encoded[j++] = '.';
        strcpy (&encoded[j], scope);
    }
    return j + scope_len;
}

int decode_netbios_name (const char *encoded_name, char *decoded_name, size_t decoded_size)
{
    size_t i, j, len;
    
    len = strlen (encoded_name);
    if (len < NETBT_NAME_LENGTH * 2 || decoded_size < len - NETBT_NAME_LENGTH + 1)
        return LWINS_ERR;
    
    i = j = 0;
    while (i < NETBT_NAME_LENGTH * 2)
    {
        if (encoded_name[i] < 'A' || encoded_name[i] > 'A' + 0xF)
            return LWINS_ERR;
        
        ASSERT (j < decoded_size);
        decoded_name[j] = ((encoded_name[i++] - 'A') & 0x0F) << 4;
        decoded_name[j++] |= (encoded_name[i++] - 'A') & 0x0F;
    }
    
    /* append scope */
    strcpy (&decoded_name[j], &encoded_name[i]);
    return 1;
}

static size_t calculated_compressed_size (const char *domain_name)
{
    ssize_t last_dot;
    size_t label_len, i, len, size = 0;
    
    label_len = 0;
    last_dot = -1;
    len = strlen (domain_name);
    for (i = 0; i < len; i++)
    {
        if (domain_name[i] == '.')
        {
            if (i == 0 || last_dot == (ssize_t)(i - 1))
                return LWINS_ERR;
            last_dot = i;
            if (label_len > 0)
                size += 1 + label_len;
            label_len = 0;
        }
        else if (++label_len > 63)
            return LWINS_ERR;
    }
    
    if (label_len > 0)
        size += 1 + label_len;
    
    if (size > NETBT_COMPRESSED_MAX)
        return LWINS_ERR;
    
    return size + 1;
}

static int compress_domainname (const char *domain_name, char *compressed_name, size_t compressed_size)
{
    ssize_t last_dot;
    size_t i, len, label_len, size = 0;
    const char *label;
    
    if (!is_valid_domain_name (domain_name))
        return LWINS_ERR;
    memset (compressed_name, 0, compressed_size);
    
    ASSERT(compressed_size >= calculated_compressed_size (domain_name));
    
    label_len = 0;
    label = domain_name;
    last_dot = -1;
    len = strlen (domain_name);
    for (i = 0; i < len; i++)
    {
        if (domain_name[i] == '.')
        {
            if (i == 0 || last_dot == (ssize_t)(i - 1))
                return LWINS_ERR;
            last_dot = i;
            if (label_len > 0)
            {
                compressed_name[size++] = label_len;
                memcpy (&compressed_name[size], label, label_len);
                size += label_len;
            }
            label_len = 0;
        }
        else if (++label_len > 63)
            return LWINS_ERR;
    }
    
    if (label_len > 0)
    {
        compressed_name[size++] = label_len;
        memcpy (&compressed_name[size], label, label_len);
        size += label_len;
    }
    
    compressed_name[size++] = 0;
    if (size > NETBT_COMPRESSED_MAX)
        return LWINS_ERR;
    
    return size;
}

static void netbt_free_resources (struct netbt_resource *resources, unsigned int count)
{
    unsigned int i;
    
    for (i = 0; i < count; i++)
    {
        if (resources[i].rname)
        {
            lwins_free (resources[i].rname);
            resources[i].rname = NULL;
        }
        
        if (resources[i].rdata)
        {
            lwins_free (resources[i].rdata);
            resources[i].rdata = NULL;
        }
    }
}

void netbt_free_packet (struct netbt_packet *packet)
{
    u16 i;
    
    if (packet->questions)
    {
        for (i = 0; i < packet->header.qscnt; i++)
        {
            if (packet->questions[i].qname)
            {
                lwins_free (packet->questions[i].qname);
                packet->questions[i].qname = 0;
            }
        }
        
        lwins_free (packet->questions);
    }
    
    if (packet->answers)
        netbt_free_resources (packet->answers, packet->header.ancnt);
    if (packet->authority)
        netbt_free_resources (packet->authority, packet->header.nscnt);
    if (packet->additional)
        netbt_free_resources (packet->additional, packet->header.arcnt);
    
    memset (packet, 0, sizeof (*packet));
}

static int netbt_find_label (struct netbt_packet_context *ctx, unsigned int find)
{
    u16 i;
    
    ASSERT (find <= 0xFFFF);
    
    for (i = 0; i < ctx->labels_cnt; i++)
    {
        if (ctx->labels[i] == (u16)find)
            return 1;
    }
    
    return LWINS_ERR;
}

static int netbt_add_label (struct netbt_packet_context *ctx, unsigned int add)
{
    u16 i;
    
    ASSERT (add <= 0xFFFF);
    
    for (i = 0; i < ctx->labels_cnt; i++)
    {
        if (ctx->labels[i] == (u16)add)
            return 1;
    }
    
    if (ctx->labels_cnt < NETBT_MAX_LABELS)
    {
        ctx->labels[ctx->labels_cnt++] = (u16)add;
        return 1;
    }
    
    return LWINS_ERR;
}

static int netbt_parse_compressed_name (struct netbt_packet_context *ctx, char **name, int decode)
{
    const unsigned char *start;
    char *allocated;
    char decompressed[NETBT_COMPRESSED_MAX + 1];
    u8 label_len, ptrs_followed;
    u16 last_ptr, decompressed_pos;
    size_t rem, prevpos, curpos, alloc_size;
    u16 readcnt = 0;
    
    memset (decompressed, 0, sizeof (decompressed));
    
    start = netbt_packet_current (ctx);
    prevpos = netbt_packet_position (ctx);
    last_ptr = 0;
    ptrs_followed = 0;
    decompressed_pos = 0;
    
    do
    {
        rem = netbt_packet_data_remaining (ctx);
        if (rem < 1)
            return LWINS_ERR;
        
        label_len = netbt_packet_read_u8 (ctx);
        if (++readcnt > NETBT_COMPRESSED_MAX)
            return LWINS_ERR;
        
        curpos = netbt_packet_position (ctx) - 1;
        if (ptrs_followed == 0)
        {
            if (curpos > 0xFFFF)
                return LWINS_ERR;
            if (!netbt_add_label (ctx, curpos & 0xFFFF))
                return LWINS_ERR;
        }
        if (label_len != 0)
        {
            if (label_len & 0xC0)
            {
                if ((label_len & 0xC0) != 0xC0)
                    return LWINS_ERR;
                
                rem = netbt_packet_data_remaining (ctx);
                if (rem < 1)
                    return LWINS_ERR;
                
                if (!last_ptr)
                    prevpos = netbt_packet_position (ctx) + 1;
                last_ptr = (u16)(label_len & ~0xC0) << 8;
                last_ptr |= netbt_packet_read_u8 (ctx);
                if (!last_ptr || last_ptr == curpos)
                    return LWINS_ERR;
                if (!netbt_find_label (ctx, last_ptr))
                    return LWINS_ERR;
                if (ptrs_followed++ > 10)
                    return LWINS_ERR;
                
                netbt_set_packet_position (ctx, (size_t)last_ptr);
                continue;
            }
            
            if (rem < 1 + (size_t)label_len)
                return LWINS_ERR;
            
            if (decompressed_pos != 0)
            {
                if (decompressed_pos >= sizeof (decompressed) - 1)
                    return LWINS_ERR;
                decompressed[decompressed_pos++] = '.';
            }
            
            if (decompressed_pos >= sizeof (decompressed) - label_len)
                return LWINS_ERR;
            
            netbt_packet_read_data (ctx, (unsigned char *)&decompressed[decompressed_pos], label_len);
            decompressed_pos += label_len;
            readcnt += label_len;
            if (readcnt > NETBT_COMPRESSED_MAX)
                return LWINS_ERR;
            if (ptrs_followed == 0)
                prevpos = netbt_packet_position (ctx);
        }
        else if (ptrs_followed == 0)
            prevpos = netbt_packet_position (ctx);
        
    } while (label_len != 0);
    
    netbt_set_packet_position (ctx, prevpos);
    
    if (decode)
    {
        alloc_size = strlen (decompressed) - 16;
        allocated = lwins_alloc (alloc_size + 1);
        if (!allocated)
            return LWINS_ERR;
        
        if (!decode_netbios_name (decompressed, allocated, alloc_size + 1))
        {
            lwins_free (allocated);
            return LWINS_ERR;
        }
    }
    else
    {
        alloc_size = strlen (decompressed);
        allocated = lwins_alloc (alloc_size + 1);
        if (!allocated)
            return LWINS_ERR;
        strcpy (allocated, decompressed);
    }
    
    *name = allocated;
    return 1;
}

static void netbt_build_raw_packet_header (struct netbt_packet_context *ctx, struct netbt_header *header)
{
    u16 tmpu16;
    
    netbt_packet_put_u16 (ctx, header->tran_id);
    
    tmpu16 = header->response_code & 0xF;
    if (header->broadcast)
        tmpu16 |= (1 << 4);
    if (header->recursion_available)
        tmpu16 |= (1 << 7);
    if (header->recursion_desired)
        tmpu16 |= (1 << 8);
    if (header->truncation)
        tmpu16 |= (1 << 9);
    if (header->authoritive_answer)
        tmpu16 |= (1 << 10);
    tmpu16 |= (header->op_code & 0xF) << 11;
    if (header->is_response)
        tmpu16 |= (1 << 15);
    
    netbt_packet_put_u8 (ctx, (u8)((tmpu16 >> 8) & 0xFF));
    netbt_packet_put_u8 (ctx, (u8)(tmpu16 & 0xFF));
    netbt_packet_put_u16 (ctx, header->qscnt);
    netbt_packet_put_u16 (ctx, header->ancnt);
    netbt_packet_put_u16 (ctx, header->nscnt);
    netbt_packet_put_u16 (ctx, header->arcnt);
}

static void netbt_parse_raw_packet_header (struct netbt_packet_context *ctx, struct netbt_header *header)
{
    u16 tmpu16;
    
    header->tran_id = netbt_packet_read_u16 (ctx);
    
    tmpu16 = netbt_packet_read_u8 (ctx) << 8;
    tmpu16 |= netbt_packet_read_u8 (ctx);
    if (tmpu16 & (1 << 15))
        header->is_response = 1;
    header->op_code = (tmpu16 >> 11) & 0xF;
    if (tmpu16 & (1 << 10))
        header->authoritive_answer = 1;
    if (tmpu16 & (1 << 9))
        header->truncation = 1;
    if (tmpu16 & (1 << 8))
        header->recursion_desired = 1;
    if (tmpu16 & (1 << 7))
        header->recursion_available = 1;
    if (tmpu16 & (1 << 4))
        header->broadcast = 1;
    header->response_code = tmpu16 & 0xF;
    header->qscnt = netbt_packet_read_u16 (ctx);
    header->ancnt = netbt_packet_read_u16 (ctx);
    header->nscnt = netbt_packet_read_u16 (ctx);
    header->arcnt = netbt_packet_read_u16 (ctx);
}

static int netbt_build_questions (struct netbt_packet_context *ctx, struct netbt_packet *packet)
{
    char encoded[NETBT_COMPRESSED_MAX + 1];
    size_t len;
    u16 i;
    
    for (i = 0; i < packet->header.qscnt; i++)
    {
        if (!encode_netbios_name (packet->questions[i].qname, NULL, encoded, sizeof (encoded)))
            return LWINS_ERR;
        len = calculated_compressed_size (encoded);
        if (!len || netbt_packet_data_remaining (ctx) < len + (2 * sizeof (u16)))
            return LWINS_ERR;
        if (!compress_domainname (encoded, (char *)netbt_packet_current (ctx), len))
            return LWINS_ERR;
        netbt_packet_data_skip (ctx, len);
        
        netbt_packet_put_u16 (ctx, packet->questions[i].qtype);
        netbt_packet_put_u16 (ctx, packet->questions[i].qclass);
    }
    
    return 1;
}

static int netbt_parse_questions (struct netbt_packet_context *ctx, struct netbt_packet *packet)
{
    u16 i;
    
    packet->questions = lwins_alloc (sizeof (*packet->questions) * packet->header.qscnt);
    if (!packet->questions)
        return LWINS_ERR;
    
    for (i = 0; i < packet->header.qscnt; i++)
    {
        if (!netbt_parse_compressed_name (ctx, &packet->questions[i].qname, 1))
            return LWINS_ERR;
        /* Make sure we still have enough data to read */
        if ((size_t)netbt_packet_data_remaining (ctx) < 2 * sizeof (u16))
            return LWINS_ERR;
        packet->questions[i].qtype = netbt_packet_read_u16 (ctx);
        packet->questions[i].qclass = netbt_packet_read_u16 (ctx);
    }
    
    return 1;
}

static int netbt_build_resources (struct netbt_packet_context *ctx, struct netbt_resource *res, unsigned int count)
{
    char encoded[NETBT_COMPRESSED_MAX + 1];
    size_t len;
    unsigned int i;
    
    for (i = 0; i < count; i++)
    {
        if (!encode_netbios_name (res[i].rname, NULL, encoded, sizeof (encoded)))
            return LWINS_ERR;
        len = calculated_compressed_size (encoded);
        if (!len || netbt_packet_data_remaining (ctx) < len + 3 * sizeof (u16) + sizeof (u32))
            return LWINS_ERR;
        if (!compress_domainname (res[i].rname, (char *)netbt_packet_current (ctx), len))
            return LWINS_ERR;
        netbt_packet_data_skip (ctx, len);
        
        netbt_packet_put_u16 (ctx, res[i].rtype);
        netbt_packet_put_u16 (ctx, res[i].rclass);
        netbt_packet_put_u32 (ctx, res[i].rttl);
        netbt_packet_put_u16 (ctx, res[i].rdlen);
        if (res[i].rdlen > 0)
        {
            if (netbt_packet_data_remaining (ctx) < res[i].rdlen)
                return LWINS_ERR;
            netbt_packet_write_data (ctx, res[i].rdata, res[i].rdlen);
        }
    }
    
    return 1;
}

static int netbt_parse_resources (struct netbt_packet_context *ctx, struct netbt_resource **pres, unsigned int count)
{
    struct netbt_resource *res;
    unsigned int i;
    
    res = lwins_alloc (sizeof (*res) * count);
    if (!res)
        return LWINS_ERR;
    
    *pres = res;
    for (i = 0; i < count; i++)
    {
        if (!netbt_parse_compressed_name (ctx, &res[i].rname, 1))
            return LWINS_ERR;
        /* Make sure we still have enough data to read */
        if ((size_t)netbt_packet_data_remaining (ctx) < (3 * sizeof (u16)) + sizeof (u32))
            return LWINS_ERR;
        res[i].rtype = netbt_packet_read_u16 (ctx);
        res[i].rclass = netbt_packet_read_u16 (ctx);
        res[i].rttl = netbt_packet_read_u32 (ctx);
        res[i].rdlen = netbt_packet_read_u16 (ctx);
        if (res[i].rdlen > 0)
        {
            /* Make sure we still have enough data to read */
            if (netbt_packet_data_remaining (ctx) < res[i].rdlen)
                return LWINS_ERR;
            
            res[i].rdata = lwins_alloc (res[i].rdlen);
            if (!res[i].rdata)
                return LWINS_ERR;
            netbt_packet_read_data (ctx, res[i].rdata, res[i].rdlen);
        }
    }
    
    return 1;
}

int netbt_parse_raw_packet (unsigned char *raw_packet, size_t len, struct sockaddr_in *addr, struct netbt_packet *packet)
{
    struct netbt_packet_context ctx;
    
    if (len < NETBT_HEADER_SIZE)
        return LWINS_ERR;
    
    ctx.packet = raw_packet;
    ctx.length = len;
    ctx.position = 0;
    ctx.labels_cnt = 0;
    
    memset (packet, 0, sizeof (*packet));
    packet->addr = *addr;
    
    /* parse header */
    netbt_parse_raw_packet_header (&ctx, &packet->header);
    
    /* parse question records */
    if (packet->header.qscnt > 0 && !netbt_parse_questions (&ctx, packet))
    {
        netbt_free_packet (packet);
        return LWINS_ERR;
    }
    
    /* parse answer records */
    if (packet->header.ancnt > 0 && !netbt_parse_resources (&ctx, &packet->answers, packet->header.ancnt))
    {
        netbt_free_packet (packet);
        return LWINS_ERR;
    }
    
    /* parse authority records */
    if (packet->header.nscnt > 0 && !netbt_parse_resources (&ctx, &packet->authority, packet->header.nscnt))
    {
        netbt_free_packet (packet);
        return LWINS_ERR;
    }
    
    /* parse additional records */
    if (packet->header.arcnt > 0 && !netbt_parse_resources (&ctx, &packet->additional, packet->header.arcnt))
    {
        netbt_free_packet (packet);
        return LWINS_ERR;
    }
    
    return (int)ctx.position;
}

int netbt_build_raw_packet (struct netbt_packet *packet, unsigned char *raw_packet, size_t len)
{
    struct netbt_packet_context ctx;
    
    if (len < NETBT_HEADER_SIZE)
        return LWINS_ERR;
    
    memset (raw_packet, 0, len);
    
    ctx.packet = raw_packet;
    ctx.length = len;
    ctx.position = 0;
    ctx.labels_cnt = 0;
    
    /* build header */
    netbt_build_raw_packet_header (&ctx, &packet->header);
    
    /* build question records */
    if (packet->header.qscnt > 0 && !netbt_build_questions (&ctx, packet))
        return LWINS_ERR;
    
    /* build answer records */
    if (packet->header.ancnt > 0 && !netbt_build_resources (&ctx, packet->answers, packet->header.ancnt))
        return LWINS_ERR;
    
    /* build authority records */
    if (packet->header.nscnt > 0 && !netbt_build_resources (&ctx, packet->authority, packet->header.nscnt))
        return LWINS_ERR;
    
    /* build additional records */
    if (packet->header.arcnt > 0 && !netbt_build_resources (&ctx, packet->additional, packet->header.arcnt))
        return LWINS_ERR;
    
    return (int)ctx.position;
}

const char* netbt_packet_question (struct netbt_packet *packet, unsigned int idx)
{
    if (packet->questions && idx < packet->header.qscnt)
        return packet->questions[idx].qname;
    
    return NULL;
}

