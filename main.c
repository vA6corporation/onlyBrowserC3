#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include <string.h>

GtkBuilder *builder;
GtkWidget *main_window;
GtkWidget *password_dialog;
GtkWidget *password_entry;
GtkWidget *notebook;
GtkWidget *webkit_view;
GtkWidget *new_viewport;

WebKitWebView *webView;

WebKitNavigationPolicyDecision *navigation_decision;
WebKitResponsePolicyDecision *response;
WebKitNavigationAction *navigation_action;
WebKitURIRequest *uri_request;
const gchar *uri;
const gchar *uri_tmp;

void compare_str()
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("/usr/local/bin/program.txt", "r");
    if (fp == NULL)
        printf("%s\n", "no se encontro fichero");
    strcat(uri_tmp, "\n" );
    //getline (&line, &len, fp);
    while ((read = getline(&line, &len, fp)) != -1)
    {
        int value = strcmp(line, uri_tmp);
        g_print("%s -- %s",line,uri_tmp);
        if (value == 0)
        {
            webkit_web_view_load_uri(webView, uri);
            printf("%s\n", "Cumple");
        }

        else
        {
            printf("no cumple\n");
        }

        char *find_view = strstr(uri, "ve.google.com/open");
        if (find_view != 0)
            webkit_web_view_load_uri(webView, uri);

    }

}

static gboolean
decide_policy_cb (WebKitWebView *webView,
                  WebKitPolicyDecision *decision,
                  WebKitPolicyDecisionType type)
{

    switch (type)
    {
        case WEBKIT_POLICY_DECISION_TYPE_NAVIGATION_ACTION:
            navigation_decision = WEBKIT_NAVIGATION_POLICY_DECISION (decision);
            /* Make a policy decision here. */
            g_print("Abriendo link\n");
            break;
        case WEBKIT_POLICY_DECISION_TYPE_NEW_WINDOW_ACTION:
            navigation_decision = WEBKIT_NAVIGATION_POLICY_DECISION (decision);
            /* Make a policy decision here. */
            g_print("Abriendo ventana\n");
            navigation_action = webkit_navigation_policy_decision_get_navigation_action
                                                                    (navigation_decision);
            uri_request = webkit_navigation_action_get_request
                                            (navigation_action);
            uri = webkit_uri_request_get_uri (uri_request);
            uri_tmp = webkit_uri_request_get_uri (uri_request);
            //g_print("%s\n", uri);
            compare_str();
            //webkit_web_view_load_uri (webView, uri);
            break;
        case WEBKIT_POLICY_DECISION_TYPE_RESPONSE:
            response = WEBKIT_RESPONSE_POLICY_DECISION (decision);
            /* Make a policy decision here. */
            g_print("No estamos seguros\n");
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
                            gpointer user_data)
{
    g_print("hola");
    gchar pass[] = "123";
    const gchar *entry_text;
    entry_text = gtk_entry_get_text (GTK_ENTRY(password_entry));
    //strcmp es el metodo para comparar 2 arreglos de caracteres
    int check = !strcmp(entry_text, pass);

    if (check == 1)
    {
        gtk_main_quit ();
    }
}

void
on_close_button_clicked (GtkWidget *widget,
                        gpointer data)
{
    gtk_window_close (GTK_WINDOW(main_window));
}

void
on_cancel_button_clicked (GtkButton *button,
                          gpointer   user_data)
{
    gtk_widget_hide(password_dialog);
}

void
on_home_button_clicked (GtkButton *button,
                        gpointer   user_data)
{
    webkit_web_view_load_uri (webView, "http://www.munisatipo.gob.pe/");
  //system("thunar");
}

void
on_home2_button_clicked (GtkButton *button,
                        gpointer   user_data)
{
    webkit_web_view_load_uri (webView, "http://192.168.3.9/Sistema_Tramite_PCM/cInterfaseUsuario_SITD/consultaTramite.php");
  //system("thunar");
}

void
on_refresh_button_clicked (GtkButton *button,
                            gpointer   user_data)
{
    webkit_web_view_reload (webView);
}

void
on_back_button_clicked (GtkButton *button,
                      gpointer   user_data)
{
    webkit_web_view_go_back (webView);
}
// handler signals-------------------------------------------
gint
on_window_delete_event( GtkWidget *widget,
                        GdkEvent  *event,
                        gpointer   data )
{
        g_print ("delete event occurred\n");
        gtk_widget_show_all (GTK_WIDGET(password_dialog));
        return TRUE;
}

gint
on_password_dialog_delete_event(GtkWidget *widget,
                                GdkEvent  *event,
                                gpointer   user_data)
{
    gtk_widget_hide(widget);
}
//-----------------------------------------------------------
int
main (int argc, char *argv[])
{
    gtk_init (&argc, &argv);

    builder = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "/usr/local/bin/gui2.glade", NULL);

    main_window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    password_dialog = GTK_WIDGET(gtk_builder_get_object (builder, "password_dialog"));
    webkit_view = GTK_WIDGET(gtk_builder_get_object (builder, "viewport"));
    webView = WEBKIT_WEB_VIEW(webkit_web_view_new());
    password_entry = GTK_WIDGET(gtk_builder_get_object (builder, "password_entry"));
    gtk_window_fullscreen (GTK_WINDOW(main_window));
    //gtk_window_maximize (GTK_WINDOW(main_window));

    gtk_container_add (GTK_CONTAINER (webkit_view), GTK_WIDGET (webView));

    gtk_builder_connect_signals (builder,NULL);
    webkit_web_view_load_uri (webView, "http://www.munisatipo.gob.pe/");
    gtk_window_set_default_size (GTK_WINDOW(main_window),800,800);
    gtk_widget_show(GTK_WIDGET(webView));
    g_signal_connect (webView, "decide-policy", G_CALLBACK (decide_policy_cb), NULL);

    gtk_main ();

    return 0;
}
