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

#define LWINS_OPTION_LIST 1
#define LWINS_OPTION_INTEGER 2
#define LWINS_OPTION_ONOFF 3
#define LWINS_OPTION_STRING 4
#define LWINS_OPTION_LIST_IP 5

struct lwins_options
{
    const char *name;
    unsigned int type;
};

static const struct lwins_options lwins_opt[] =
{
    {"config", LWINS_OPTION_STRING}, /* must be first entry */
    {"nbns-interfaces", LWINS_OPTION_LIST},
    {"replication-interfaces", LWINS_OPTION_LIST},
    {"replication-port", LWINS_OPTION_INTEGER},
    {"replication-partners", LWINS_OPTION_LIST_IP},
    {"replication-auto-discovery", LWINS_OPTION_ONOFF},
    {"replication-packet-size-max", LWINS_OPTION_INTEGER},
    {"user", LWINS_OPTION_STRING},
    {"group", LWINS_OPTION_STRING},
};

#define LWINS_OPTION_COUNT (sizeof (lwins_opt) / sizeof (lwins_opt[0]))

static const struct lwins_options* lwins_find_option (const char *option)
{
    size_t i;
    
    for (i = 0; i < LWINS_OPTION_COUNT; i++)
    {
        if (!strcmp (option, lwins_opt[i].name))
            return &lwins_opt[i];
    }
    
    return NULL;
}

static const struct lwins_options* lwins_find_file_option (const char *option)
{
    size_t i;
    
    for (i = 1; i < LWINS_OPTION_COUNT; i++)
    {
        if (!strcmp (option, lwins_opt[i].name))
            return &lwins_opt[i];
    }
    
    return NULL;
}

int lwins_convert_to_integer (const char *param, unsigned int *ret)
{
    size_t len;
    long int value;
    char *endptr;
    
    if (ret)
        *ret = 0;
    
    len = strlen (param);
    if (len == 0)
        return 0;
    
    if (param[0] == '-')
        return 0;
    
    value = strtol (param, &endptr, 0);
    if (endptr == param || *endptr != '\0' || value == LONG_MIN || value == LONG_MAX)
        return 0;
    
    if (ret)
        *ret = (unsigned int)value;
    return 1;
}

int lwins_convert_to_bool (const char *param, int *ret)
{
    if (!strcmp (param, "on"))
    {
        if (ret)
            *ret = 1;
        return 1;
    }
    else if (!strcmp (param, "off"))
    {
        if (ret)
            *ret = 0;
        return 1;
    }
    
    return 0;
}

int lwins_convert_to_ip (const char *param, struct in_addr *ret)
{
    struct in_addr addr;
    
    if (!strcmp (param, "all") || !strcmp (param, "*"))
    {
        if (ret)
            ret->s_addr = htonl (INADDR_ANY);
        return 1;
    }
    
    if (!inet_aton (param, &addr))
        return 0;
    
    if (addr.s_addr == htonl (INADDR_ANY))
        return 0;
    
    if (ret)
        *ret = addr;
    return 1;
}

static int lwins_has_all_in_list (char **list, unsigned int cnt, const char *addparam)
{
    unsigned int i;
    
    for (i = 0; i < cnt; i++)
    {
        if (!strcmp (list[i], "all") || !strcmp (list[i], "*"))
            return 1;
    }
    
    if (cnt > 0 && (!strcmp (addparam, "all") || !strcmp (addparam, "*")))
        return 1;
    
    return 0;
}

static int lwins_add_str_to_list (char ***list, unsigned int *cnt, const char *str)
{
    size_t len;
    char **lst;
    
    len = strlen (str);
    if (!len)
        return 1;
    
    lst = *list;
    if (!lst)
    {
        lst = lwins_alloc (sizeof (*lst));
        if (!lst)
            return LWINS_ERR;
        lst[0] = lwins_alloc (len + 1);
        if (!lst[0])
        {
            lwins_free (lst);
            return LWINS_ERR;
        }
        
        strcpy (lst[0], str);
    }
    else
    {
        lst = lwins_alloc (((*cnt) + 1) * sizeof (*lst));
        if (!lst)
            return LWINS_ERR;
        memcpy (lst, *list, *cnt * sizeof (*lst));
        lst[*cnt] = lwins_alloc (len + 1);
        if (!lst[*cnt])
        {
            lwins_free (lst);
            return LWINS_ERR;
        }
        
        strcpy (lst[*cnt], str);
        lwins_free (*list);
    }
    
    *list = lst;
    (*cnt)++;
    return 1;
}

static int lwins_check_option (const char *option, struct lwins_config *cfg)
{
    const struct lwins_options *opt;
    unsigned int i;
    
    for (i = 0; i < cfg->options_cnt; i++)
    {
        if (!strcmp (cfg->options[i].name, option))
            break;
    }
    
    ASSERT (i < cfg->options_cnt);
    
    opt = lwins_find_option (option);
    if (!opt)
        return LWINS_ERR;
    
    switch (opt->type)
    {
        case LWINS_OPTION_LIST:
        case LWINS_OPTION_LIST_IP:
            return 1;
        case LWINS_OPTION_ONOFF:
        case LWINS_OPTION_INTEGER:
        case LWINS_OPTION_STRING:
            if (cfg->options[i].params_cnt == 1)
                return 1;
            else
                fprintf (stderr, "Option %s expects one parameter\n", opt->name);
            break;
    }
    
    return 0;
}

static int lwins_add_option_param (const char *option, const char *param, struct lwins_config *cfg)
{
    const struct lwins_options *opt;
    unsigned int i;
    
    for (i = 0; i < cfg->options_cnt; i++)
    {
        if (!strcmp (cfg->options[i].name, option))
            break;
    }
    
    ASSERT (i < cfg->options_cnt);
    
    opt = lwins_find_option (option);
    if (!opt)
        return LWINS_ERR;
    
    if (opt->type != LWINS_OPTION_LIST && opt->type != LWINS_OPTION_LIST_IP &&
        cfg->options[i].params_cnt >= 1)
    {
        fprintf (stderr, "Option %s expects only one parameter\n", opt->name);
        return LWINS_ERR;
    }
    
    if (opt->type == LWINS_OPTION_INTEGER)
    {
        if (!lwins_convert_to_integer (param, NULL))
        {
            fprintf (stderr, "Option %s expects integer parameter\n", opt->name);
            return LWINS_ERR;
        }
    }
    else if (opt->type == LWINS_OPTION_ONOFF)
    {
        if (!lwins_convert_to_bool (param, NULL))
        {
            fprintf (stderr, "Option %s expects \"on\" or \"off\"\n", opt->name);
            return LWINS_ERR;
        }
    }
    else if (opt->type == LWINS_OPTION_LIST_IP)
    {
        if (!lwins_convert_to_ip (param, NULL))
        {
            fprintf (stderr, "Option %s expect an IPv4 address\n", opt->name);
            return LWINS_ERR;
        }
    }
    
    if (opt->type == LWINS_OPTION_LIST || opt->type == LWINS_OPTION_LIST_IP)
    {
        if (lwins_has_all_in_list (cfg->options[i].params, cfg->options[i].params_cnt, param))
        {
            fprintf (stderr, "Option %s cannot combine \"all\" or \"*\" with any other parameter\n", opt->name);
            return LWINS_ERR;
        }
    }
    
    return lwins_add_str_to_list (&cfg->options[i].params, &cfg->options[i].params_cnt, param);
}

static int lwins_begin_option (const char *option, struct lwins_config *cfg)
{
    struct lwins_config_option *opts;
    unsigned int i;
    
    for (i = 0; i < cfg->options_cnt; i++)
    {
        if (!strcmp (cfg->options[i].name, option))
            break;
    }
    
    if (i < cfg->options_cnt)
    {
        /* found option */
        fprintf (stderr, "Option %s already defined\n", option);
        return LWINS_ERR;
    }
    
    opts = lwins_alloc ((cfg->options_cnt + 1) * sizeof (*cfg->options));
    if (!opts)
        return LWINS_ERR;
    memcpy (opts, cfg->options, cfg->options_cnt * sizeof (*cfg->options));
    memset (&opts[cfg->options_cnt], 0, sizeof (*cfg->options));
    opts[cfg->options_cnt].name = lwins_alloc (strlen (option) + 1);
    if (!opts[cfg->options_cnt].name)
    {
        lwins_free (opts);
        return LWINS_ERR;
    }
    
    strcpy (opts[cfg->options_cnt].name, option);
    cfg->options = opts;
    cfg->options_cnt++;
    return 1;
}

static int lwins_load_config_file (const char *file, struct lwins_config *cfg)
{
    static const char delims[] = " \t";
    char line[1024];
    FILE *fp;
    char *str, *tok;
    char *option, *option_cur, *tmp;
    size_t len;
    int needtok, mergeline, i, err, ret = 0;
    
    fp = fopen (file, "r");
    if (!fp)
    {
        LOGERR (LOG_ALL, "fopen");
        fprintf (stderr, "Could not open configuration file %s\n", file);
        return LWINS_ERR;
    }
    
    err = 0;
    mergeline = i = needtok = 0;
    option = option_cur = NULL;
    while ((str = fgets (line, sizeof (line), fp)))
    {
        /* strip trailing newline(s) */
        len = strlen (str);
        while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r'))
            str[--len] = '\0';
        
        i = 0;
        if (!mergeline)
            option = NULL;
        else
        {
            mergeline = 0;
            needtok = 1;
        }
        
        tok = strtok (str, delims);
        while (tok)
        {
            if (tok[0])
            {
                if (i == 0 && (tok[0] == '#' || tok[0] == ';'))
                    break;
                
                if (i == 0 && !option && !lwins_find_file_option (tok))
                {
                    fprintf (stderr, "Unknown option: %s\n", tok);
                    err = 1;
                    break;
                }
                
                if (i == 0 && !option)
                {
                    option = tok;
                    if (!lwins_begin_option (option, cfg))
                    {
                        err = 1;
                        break;
                    }
                }
                else
                {
                    if (tok[0] == '\\' && tok[1] == '\0' && tok == str + len - 1)
                    {
                        /* Trailing backslash means parameters continue at next line */
                        mergeline = 1;
                        
                        tmp = lwins_alloc (strlen (option) + 1);
                        if (!tmp)
                        {
                            err = 1;
                            break;
                        }
                        strcpy (tmp, option);
                        
                        if (option_cur)
                        {
                            lwins_free (option_cur);
                            option_cur = NULL;
                        }
                        
                        option = option_cur = tmp;
                    }
                    else if (!lwins_add_option_param (option, tok, cfg))
                    {
                        err = 1;
                        break;
                    }
                }
                
                i++;
                needtok = 0;
            }
            tok = strtok (NULL, delims);
        }
        
        if (err)
            break;
        
        if (needtok && i == 0)
            break;
        
        if (option && !mergeline && !lwins_check_option (option, cfg))
        {
            err = 1;
            break;
        }
    }
    
    if (option_cur)
        lwins_free (option_cur);
    
    if (!err)
    {
        if (ferror (fp))
            fprintf (stderr, "Could not read configuration file %s\n", file);
        else if (feof (fp))
        {
            if (mergeline)
                needtok = 1;
            else if (!needtok)
                ret = 1;
        }
    }
    
    if (needtok)
        fprintf (stderr, "Unexpected end of file\n");
    
    fclose (fp);
    return ret;
}

static int lwins_is_config_option (const char *arg)
{
    if (!strcmp (&arg[2], lwins_opt[0].name))
        return 1;
    return 0;
}

static int lwins_is_valid_arg_option (const char *arg)
{
    size_t len;
    
    len = strlen (arg);
    if (len <= 2 || arg[0] != '-' || arg[1] != '-')
        return LWINS_ERR;
    
    if (lwins_find_option (&arg[2]))
        return 1;
    
    return LWINS_ERR;
}

static int lwins_get_option_params (int idx, int argc, const char *argv[])
{
    const struct lwins_options *opt;
    int cnt = 0;
    
    opt = lwins_find_option (&argv[idx][2]);
    if (!opt)
        return LWINS_ERR;
    
    idx++;
    switch (opt->type)
    {
        case LWINS_OPTION_LIST:
        case LWINS_OPTION_LIST_IP:
            while (idx < argc)
            {
                if (strlen (argv[idx]) >= 2 && argv[idx][0] == '-' && argv[idx][1] == '-')
                    break;
                cnt++;
            }
            break;
        case LWINS_OPTION_INTEGER:
            if (idx >= argc || !lwins_convert_to_integer (argv[idx], NULL))
            {
                fprintf (stderr, "Option %s expects integer parameter\n", opt->name);
                return -1;
            }
            cnt = 1;
            break;
        case LWINS_OPTION_ONOFF:
            if (idx >= argc || !lwins_convert_to_bool (argv[idx], NULL))
            {
                fprintf (stderr, "Option %s expects \"on\" or \"off\"\n", opt->name);
                return -1;
            }
            cnt = 1;
            break;
        case LWINS_OPTION_STRING:
            if (idx >= argc)
            {
                fprintf (stderr, "Option %s expects one parameter\n", opt->name);
                return -1;
            }
            cnt = 1;
            break;
    }
    
    return cnt;
}

void lwins_free_config (struct lwins_config *cfg)
{
    unsigned int i, j;
    
    if (cfg->options)
    {
        for (i = 0; i < cfg->options_cnt; i++)
        {
            if (cfg->options[i].name)
                lwins_free (cfg->options[i].name);
            if (cfg->options[i].params)
            {
                for (j = 0; j < cfg->options[i].params_cnt; j++)
                {
                    if (cfg->options[i].params[j])
                        lwins_free (cfg->options[i].params[j]);
                }
                
                lwins_free (cfg->options[i].params);
            }
        }
        
        lwins_free (cfg->options);
    }
    
    memset (cfg, 0, sizeof (*cfg));
}

int lwins_read_config_args (int argc, const char *argv[], struct lwins_config *cfg)
{
    int j, i = 1;
    int cnt, nopts = 0;
    const char *config = NULL;
    
    memset (cfg, 0, sizeof (cfg));
    
    while (i < argc)
    {
        if (!lwins_is_valid_arg_option (argv[i]))
        {
            fprintf (stderr, "Unknown option: %s\n", argv[i]);
            return LWINS_ERR;
        }
        
        cnt = lwins_get_option_params (i, argc, argv);
        if (cnt < 0)
            return LWINS_ERR;
        
        nopts++;
        
        if (lwins_is_config_option (argv[i]))
        {
            if (nopts > 1)
            {
                fprintf (stderr, "--config cannot be combined with other options\n");
                return LWINS_ERR;
            }
            
            config = argv[i + 1];
        }
        else
        {
            if (config)
            {
                fprintf (stderr, "--config cannot be combined with other options\n");
                return LWINS_ERR;
            }
            
            if (!lwins_begin_option (&argv[i][2], cfg))
                return LWINS_ERR;
            
            for (j = 0; j < cnt; j++)
            {
                if (!lwins_add_option_param (&argv[i][2], argv[j + i + 1], cfg))
                    return LWINS_ERR;
            }
        }
        
        i += 1 + cnt;
    }
    
    if (config)
        return lwins_load_config_file (config, cfg);
    
    return 1;
}

int lwins_get_config_param (struct lwins_config *cfg, const char *option, unsigned int idx, const char **value)
{
    unsigned int i;
    
    *value = NULL;
    
    for (i = 0; i < cfg->options_cnt; i++)
    {
        if (!strcmp (cfg->options[i].name, option))
        {
            if (idx >= cfg->options[i].params_cnt)
                return 0;
            
            *value = cfg->options[i].params[idx];
            return 1;
        }
    }
    
    return 0;
}

int lwins_get_config_param_long (struct lwins_config *cfg, const char *option, unsigned int idx, unsigned long *value)
{
    const char *strval;
    int ret;
    
    ret = lwins_get_config_param (cfg, option, idx, &strval);
    if (ret)
        *value = (unsigned int)atol (strval);
    return ret;
}

int lwins_config_get_option_param_cnt (struct lwins_config *cfg, const char *option)
{
    unsigned int i;
    
    for (i = 0; i < cfg->options_cnt; i++)
    {
        if (!strcmp (cfg->options[i].name, option))
            return cfg->options[i].params_cnt;
    }
    
    return -1;
}

int lwins_config_option_has_param (struct lwins_config *cfg, const char *option, const char *param)
{
    unsigned int i, j;
    
    for (i = 0; i < cfg->options_cnt; i++)
    {
        if (!strcmp (cfg->options[i].name, option))
        {
            for (j = 0; j < cfg->options[i].params_cnt; j++)
            {
                if (!strcmp (param, cfg->options[i].params[j]))
                    return 1;
            }
            break;
        }
    }
    
    return 0;
}

