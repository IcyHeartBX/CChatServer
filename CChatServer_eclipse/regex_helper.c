#include "regex_helper.h"

gchar *get_node(const char *content, const char *regtxt)
{
	GRegex *regex;
	GMatchInfo *match_info;
	GError *error = NULL;
	regex = g_regex_new(regtxt, 1 << 2, 0, NULL);
	g_regex_match(regex, content, 0, &match_info);

	gchar *res;
	if(g_match_info_matches(match_info))
	{
		res = g_match_info_fetch(match_info, 0);
	}
	else
	{
		res = "";
	}
	g_match_info_free(match_info);
	g_regex_unref(regex);
	return res;
}
