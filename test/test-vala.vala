
class TestApp : GLib.Application, GLib.Initable
{
    private GSound.Context ctx;

    construct
    {
        GLib.Environment.set_application_name ("GSound test programme");
    }
    
    public TestApp() throws GLib.Error
    {
        Object(application_id: "net.tristanb.GSoundTest");
        init();
    }

    private bool init(Cancellable? cancellable = null) throws Error
    {
        ctx = new GSound.Context(cancellable);

        return true;
    }

    protected override void activate()
    {
        play_sound.begin();
        hold();
    }

    private async void play_sound() throws GLib.Error
    {
        yield ctx.play_full(null,
                            GSound.Attribute.MEDIA_FILENAME, "/usr/share/sounds/alsa/Front_Center.wav");
        release();
    }

}

void main(string[] args)
{
    try {
        var app = new TestApp();
        app.run(args);
    } catch (GLib.Error e) {
        critical("Caught error: %s", e.message);
    }
}