#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include <string.h>
#include <stdlib.h>

GtkBuilder *builder;
GtkWidget *main_window;
GtkWidget *password_entry;
GtkWidget *notebook;
GtkWidget *webkit_view;
GtkRevealer *uri_revealer;
GtkRevealer *password_revealer;
GtkEntry *url_entry;

WebKitWebView *webView;

WebKitNavigationPolicyDecision *navigation_decision;
WebKitResponsePolicyDecision *response;
WebKitNavigationAction *navigation_action;
WebKitURIRequest *uri_request;
const gchar *current_uri;
gchar url[50];
char *line = NULL;
char line2[50];

void remove_char (char str[]) {
    int count = (strlen (str));
    int i = 0;
    while (str[i] != '\n') {
        line2[i] = str[i];
        i ++;
        }
        line2[i] = '\0';
    }

gboolean
compare_uri () {
    FILE * fp;
    size_t len = 0;
    ssize_t read;

    fp = fopen("/home/vampirodx/.local/muniSatipo/url_aceptados.txt", "r");
    if (fp == NULL) {
        printf("%s\n", "no se encontro fichero");
    }

    while ((read = getline(&line, &len, fp)) != -1) {
        remove_char (line);
        int value1 = strcmp(line2, current_uri);
        int value2 = strstr(current_uri, line2);
        if (value1 == 0 || value2 != 0) {
            return TRUE;
        }
    }
    return FALSE;
}

compare_uri2 () {
    FILE * fp;
    size_t len = 0;
    ssize_t read;

    fp = fopen("/home/vampirodx/.local/muniSatipo/url_denegados.txt", "r");
    if (fp == NULL) {
        printf("%s\n", "no se encontro fichero");
    }

    while ((read = getline(&line, &len, fp)) != -1) {
        remove_char (line);
        int value1 = strcmp(line2, current_uri);
        int value2 = strstr(current_uri, line2);
        if (value1 == 0 || value2 != 0) {
            return TRUE;
        }
    }
    return FALSE;
}

static gboolean
decide_policy_cb (WebKitWebView *webView,
                  WebKitPolicyDecision *decision,
                  WebKitPolicyDecisionType type)
{

    switch (type) {
        case WEBKIT_POLICY_DECISION_TYPE_NAVIGATION_ACTION:
            navigation_decision = WEBKIT_NAVIGATION_POLICY_DECISION (decision);
            g_print("\n\nAbriendo link----------------\n");
            navigation_action = webkit_navigation_policy_decision_get_navigation_action
                                                                    (navigation_decision);
            uri_request = webkit_navigation_action_get_request
                                            (navigation_action);
            current_uri = webkit_uri_request_get_uri (uri_request);

            //current_uri = webkit_web_view_get_uri (webView);
            g_print ("%s", current_uri);
            gboolean compare = compare_uri ();
            gboolean compare2 = compare_uri2 ();
            g_print ("|%i - %i|", compare, compare2);
            g_print (current_uri);
            if (compare == TRUE && compare2 == FALSE) {
                g_print ("Cargado>> : %s", current_uri);
                webkit_policy_decision_use (decision);
                gtk_entry_set_text (url_entry, current_uri);
                break;
            } else {
                webkit_policy_decision_ignore (decision);
            }
            break;
        case WEBKIT_POLICY_DECISION_TYPE_NEW_WINDOW_ACTION:
            navigation_decision = WEBKIT_NAVIGATION_POLICY_DECISION (decision);
            g_print("\n\nAbriendo ventana------------------\n");
            navigation_action = webkit_navigation_policy_decision_get_navigation_action
                                                                    (navigation_decision);
            uri_request = webkit_navigation_action_get_request
                                            (navigation_action);
            current_uri = webkit_uri_request_get_uri (uri_request);
            g_print ("%s\n", current_uri);
            //gboolean compare2 = compare_uri ();
            //webkit_policy_decision_use (decision);
            webkit_web_view_load_uri (webView, current_uri);
            //gtk_entry_set_text (urlEntry, current_uri);
            break;
        case WEBKIT_POLICY_DECISION_TYPE_RESPONSE:
            response = WEBKIT_RESPONSE_POLICY_DECISION (decision);
            g_print("\nNo estamos seguros\n");
            //webkit_policy_decision_ignore (decision);
            break;
        default:
            /* Making no decision results in webkit_policy_decision_use(). */
            return FALSE;
    }
    return TRUE;
}

// handler button signals
void
on_aceptar_button_clicked (GtkButton *button,
                            gpointer user_data) {
    gchar pass[] = "123";
    const gchar *entry_text;
    entry_text = gtk_entry_get_text (GTK_ENTRY(password_entry));
    //strcmp es el metodo para comparar 2 arreglos de caracteres
    int check = !strcmp(entry_text, pass);

    if (check == 1) {
        gtk_main_quit ();
    } else {
        gtk_entry_set_text (GTK_ENTRY(password_entry), "");
        gtk_widget_grab_focus (password_entry);
    }
}

void
on_close_button_clicked (GtkWidget *widget,
                        gpointer data) {
    gtk_window_close (GTK_WINDOW(main_window));
}

void
on_cancel_button_clicked (GtkButton *button,
                          gpointer   user_data) {
    gtk_revealer_set_reveal_child (password_revealer, FALSE);
}

void
on_home_button_clicked (GtkButton *button,
                        gpointer   user_data) {
    webkit_web_view_load_uri (webView, "http://www.munisatipo.gob.pe/");
}

void
on_home2_button_clicked (GtkButton *button,
                        gpointer   user_data) {
    webkit_web_view_load_uri (webView, "http://192.168.3.9/Sistema_Tramite_PCM/cInterfaseUsuario_SITD/consultaTramite.php");
}

void
on_refresh_button_clicked (GtkButton *button,
                            gpointer   user_data) {
    webkit_web_view_reload (webView);
}

void
on_back_button_clicked (GtkButton *button,
                      gpointer   user_data) {
    webkit_web_view_go_back (webView);
}
// handler signals-------------------------------------------
gint
on_window_delete_event (GtkWidget *widget,
                        GdkEvent  *event,
                        gpointer   data ) {
        g_print ("delete event occurred\n");
        gtk_revealer_set_reveal_child (password_revealer, TRUE);
        gtk_widget_grab_focus (password_entry);
        return TRUE;
}

//-----------------------------------------------------------
int
main (int argc, char *argv[]) {
    gtk_init (&argc, &argv);

    builder = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "/home/vampirodx/.local/muniSatipo/gui2.glade", NULL);

    main_window = GTK_WIDGET (gtk_builder_get_object (builder, "window"));
    webkit_view = GTK_WIDGET (gtk_builder_get_object (builder, "viewport"));
    webView = WEBKIT_WEB_VIEW (webkit_web_view_new ());
    password_entry = GTK_WIDGET (gtk_builder_get_object (builder, "password_entry"));
    url_entry = GTK_ENTRY (gtk_builder_get_object (builder, "url_entry"));
    password_entry = GTK_WIDGET (gtk_builder_get_object (builder, "password_entry"));
    uri_revealer = GTK_REVEALER (gtk_builder_get_object (builder, "uri_revealer"));
    password_revealer = GTK_REVEALER (gtk_builder_get_object (builder, "password_revealer"));
    gtk_revealer_set_reveal_child (uri_revealer, FALSE);
    gtk_revealer_set_reveal_child (password_revealer, FALSE);

    gtk_container_add (GTK_CONTAINER (webkit_view), GTK_WIDGET (webView));

    gtk_builder_connect_signals (builder,NULL);
    webkit_web_view_load_uri (webView, "http://www.munisatipo.gob.pe/");
    gtk_window_set_default_size (GTK_WINDOW(main_window),800,800);
    gtk_widget_show(GTK_WIDGET (webView));
    g_signal_connect (webView, "decide-policy", G_CALLBACK (decide_policy_cb), NULL);
    system ("shutdown -h 17:30");//apagado del systema

    //gtk_window_fullscreen (GTK_WINDOW(main_window));
    gtk_window_maximize (GTK_WINDOW (main_window));

    gtk_main ();

    return 0;
}
