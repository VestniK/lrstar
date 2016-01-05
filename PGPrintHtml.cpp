
#ifdef LRSTAR

      #include "ComGlobal.h"
		#include "PGGlobal.h"

      FILE* fp;
      int   head_on;

/*--- HTML_GRM --------------------------------------------------------------*/

void  PGPrintHtml::PrintHtml ()
{
      char fid [MAX_PATH];
      int  line_length;
      int  h, p, t, s, l, nh;

      head_on = 0;
      strcpy (fid, gdn);
      strcat (fid, gfn);
      strcat (fid, gft);
      strcat (fid, ".grammar.html");
		if (optn[PG_HTML] == 0) 
		{
			unlink (fid);
			return;
		}
      fp = fopen (fid, "w");
      if (fp == NULL)
      {  
         n_errors++;
         prt_log ("\nFile %s cannot be created.\n", fid);
         PG::Terminate (1);
      }
   
		fprintf (fp, "<head>\n");
		fprintf (fp, "<title>%s grammar.</title>\n", gfn);
		fprintf (fp, "<meta name=\"description\" content=\"%s grammar.\">\n", gfn);
		fprintf (fp, "<style>\n");
		fprintf (fp, "a:link    { color: #000000; text-decoration: none;      };\n");
		fprintf (fp, "a:visited { color: #000000; text-decoration: none;      };\n");
		fprintf (fp, "a:hover   { color: #000000; text-decoration: underline; };\n");
		fprintf (fp, "a:active  { color: #000000; text-decoration: underline; };\n");
		fprintf (fp, "</style>\n");
		fprintf (fp, "</head>\n");
		fprintf (fp, "<basefont face=verdana>\n");
		fprintf (fp, "<body bgcolor=\"white\" text=\"black\" link=\"blue\" vlink=\"blue\" alink=\"blue\">\n");
		fprintf (fp, "<pre>            <b>%s</b> grammar (output from <a href=\"http://lrtec.com/\">LRSTAR</a>).\n</pre>\n", gfn);

		if (optn[PG_HTML] == 0) 
		{
			fprintf (fp, "<pre>\n            'h' option was not specified.</pre>\n");
			goto Ret;
		}

   // NONTERMINALS. 
		nh = N_heads;
      for (h = 0; h < nh; h++)
      {
			fprintf (fp, "<a name=\"%s\"><pre>", head_name[h]);
         fprintf (fp, "%4d        ", h);
         ph_head (h, "</a>\n");
         for (p = F_prod [h]; p < F_prod [h+1]; p++)
         {
            line_length = 16;
            fprintf (fp, "    %4d     -&gt ", p);
            for (t = F_tail [p]; t < F_tail [p+1]; t++)
            {
               s = Tail [t];
					if (s >= 0) l = strlen (term_name[s]);
					else        l = strlen (head_name[-s]);
               if (line_length > 16 && line_length + l > 100)
               {
                  fprintf (fp, "\n                ");
                  line_length = 16;
	            }  
               line_length += ph_sym (s, " ");
				}
				fprintf (fp, "\n");
         }
			fprintf (fp, "</pre>\n"); 
      }

		fprintf (fp, "<pre>\n            End of <b>%s</b> grammar listing.\n</pre>\n", gfn);

		for (h = 0; h < 30; h++)
		{
			fprintf (fp, "<pre><br></pre>\n"); 
		}

Ret:	fprintf (fp, "</body>\n");
		fprintf (fp, "</html>\n\n");
		fclose (fp);
}

int	PGPrintHtml::ph_head (int s, char *sp)
{
      int l;
	   head_on = 1;
      l = ph_sym (-s, sp);
		head_on = 0;
		return (l);
}

int	PGPrintHtml::ph_sym (int s, char *sp)
{
      char *p;
      if (s >= 0 && head_on == 0) 
		{
			p = term_name[s];
		}
      else
		{
			p = head_name[-s];
		}
		return (ph_str (s, p, sp));
}

int	PGPrintHtml::ph_str (int s, char *p, char *sp) 
{
      int i, j, l;
      char string[1024];

		j = 0;
		l = strlen (p);
      if (l > 1023) l = 1023;
		for (i = 0; i < l; i++)
		{
			if (p[i] == '<')
			{
				string[j++] = '&';
				string[j++] = 'l';
				string[j++] = 't';
				string[j++] = ';';
			}
			else if (p[i] == '>')
			{
				string[j++] = '&';
				string[j++] = 'g';
				string[j++] = 't';
				string[j++] = ';';
			}
			else
			{
				string[j++] = p[i];
			}
		}
		string [j] = 0;
      if (head_on)   fprintf (fp, "<b>");
		else if (s < 0)     fprintf (fp, "<a href=\"#%s\">", string);
      else if (*p == '<') fprintf (fp, "<font color=#df0000>");
      else if (*p == '{') fprintf (fp, "<font color=#df0000>");
      else if (*p == '\'');
      else if (s  >   0 ) fprintf (fp, "<font color=#0000df>");

      fprintf (fp, "%s", string);

      if (head_on)   fprintf (fp, "</b>");
		else if (s < 0)     fprintf (fp, "</a>");
      else if (*p == '<') fprintf (fp, "</font>");
      else if (*p == '{') fprintf (fp, "</font>");
      else if (*p == '\'');
      else if (s  >   0 ) fprintf (fp, "</font>");
      fprintf (fp, "%s", sp);
      return (strlen(p) + strlen(sp));
}

void  PGPrintHtml::ph_spaces (int ns)
{
      if (ns < 0)
      {
         ns = 5 + (ns % 5);
         if (ns == 5) return;
      }
      spaces [ns] = 0;
      fprintf (fp, "%s", spaces);
      spaces [ns] = ' ';
}

/*--- HTML_GRM --------------------------------------------------------------*/

#endif
