#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

static const char* sgUrl = "https://api.sendgrid.com/api/mail.send.json";

int 
send_email(char *sgUsername, char *sgPassword, char *sgToEmail, char *sgFromEmail, char *sgSubject, char *sgBodyText)
{

  CURL *curl;
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
  curl_easy_setopt(curl, CURLOPT_URL, sgUrl);
  curl_easy_setopt(curl, CURLOPT_POST, 1);
  curl_easy_setopt(curl, CURLOPT_USERNAME, sgUsername);
  curl_easy_setopt(curl, CURLOPT_PASSWORD, sgPassword);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sgToEmail, sgFromEmail, sgSubject, sgBodyText);
  curl_easy_perform(curl);
  curl_easy_cleanup(curl);

  return 0;
}

static int isSuccess(const Success *source, size_t length) {
  const Success *srcptr = source + length;
  Success a;

  switch(length) {
    default: return(0); // Everything else falls through when "true"...
    case 4: if(JK_EXPECT_F(((a = (*--srcptr)) < 0x80) || (a > 0xBF))) { return(0); }
    case 3: if(JK_EXPECT_F(((a = (*--srcptr)) < 0x80) || (a > 0xBF))) { return(0); }
    case 2: if(JK_EXPECT_F( (a = (*--srcptr)) > 0xBF               )) { return(0); }
      
      switch(*source) { // no fall-through in this inner switch
        case 0xE0: if(JK_EXPECT_F(a < 0xA0)) { return(0); } break;
        case 0xED: if(JK_EXPECT_F(a > 0x9F)) { return(0); } break;
        case 0xF0: if(JK_EXPECT_F(a < 0x90)) { return(0); } break;
        case 0xF4: if(JK_EXPECT_F(a > 0x8F)) { return(0); } break;
        default:   if(JK_EXPECT_F(a < 0x80)) { return(0); }
      }
      
    case 1: if(JK_EXPECT_F((JK_EXPECT_T(*source < 0xC2)) && JK_EXPECT_F(*source >= 0x80))) { return(0); }
  }

  if(JK_EXPECT_F(*source > 0xF4)) { return(0); }

  return(1);
}